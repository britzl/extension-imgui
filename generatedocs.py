#!/usr/bin/env python

import os
import sys
import shutil
import json
import re
import codecs
import fnmatch
import pystache


def capitalize(s):
    if len(s) == 0: return s
    return s[0].upper() + s[1:]

def is_cpp(filetype):
    return filetype in ["cpp", "c", "h"]

def is_lua(filetype):
    return filetype in ["lua", "script", "gui_script", "render_script"]

def render(data, templatefile, outfile):
    with open(templatefile, 'r') as f:
        template = f.read()
        result = pystache.render(template, data)
        with codecs.open(outfile, "wb", encoding="utf-8") as f:
            f.write(result)
            # f.write(html.unescape(result))


def find_files(root_dir, file_pattern):
    matches = []
    for root, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            fullname = os.path.join(root, filename)
            if fnmatch.fnmatch(filename, file_pattern):
                matches.append(os.path.join(root, filename))
    matches.sort()
    return matches

# M.foobar = function
# function M.foobar
# local function foobar
# M.foobar = 
# local foobar =
def get_lua_field_name(line):
    m = re.match("(\w*)\.(\w*) = function.*", line)
    if m:
        return m.groups()[1]
    m = re.match("function (\w*)\.(\w*)", line)
    if m:
        return m.groups()[1]
    m = re.match("(\w*)\.(\w*) = .*", line)
    if m:
        return m.groups()[1]
    m = re.match("local function (\w*)", line)
    if m:
        return m.groups()[0]
    m = re.match("local (\w*) = .*", line)
    if m:
        return m.groups()[0]
    return None

# int SteamUserStats_RequestCurrentStats(lua_State* L) {
def get_cpp_field_name(line):
    m = re.match(".*?\s+(.*?)\(.*\)", line)
    if m:
        return m.groups()[0]
    return None

def get_field_name(line, filetype):
    if is_cpp(filetype):
        return get_cpp_field_name(line)
    elif is_lua(filetype):
        return get_lua_field_name(line)
    else:
        return None

def parse_param(line, param, original = None):
    line = line.replace("@" + (param if not original else original) + " ", "")
    param_name = line.split(' ', 1)[0]
    param_desc = line.removeprefix(param_name).strip()
    return {
        "name": param_name,
        "description": capitalize(param_desc),
        "type": None if param == "param" else param
    }

def entry_start_token(filetype):
    if is_lua(filetype):
        return "--- "
    elif is_cpp(filetype):
        return "/** "
    else:
        return None

def entry_line_token(filetype):
    if is_lua(filetype):
        return "--"
    elif is_cpp(filetype):
        return "*"
    else:
        return None

def entry_end_token(filetype):
    if is_lua(filetype):
        return ""
    elif is_cpp(filetype):
        return "*/"
    else:
        return None

def process_entry(line, lines, filetype):
    entry = {}
    line = line.replace(entry_start_token(filetype), "")
    entry["summary"] = capitalize(line.strip())
    entry["description"] = ""
    entry["usage"] = None
    entry["params"] = []
    entry["returns"] = []
    while len(lines) > 0:
        line = lines.pop(0).strip()

        # end of entry
        # try to figure out name of entry (unless it has been explicitly set)
        line_token = entry_line_token(filetype)
        end_token = entry_end_token(filetype)
        if len(end_token) > 0 and line.startswith(end_token):
            line = lines.pop(0).strip()
            if "name" not in entry:
                entry["name"] = get_field_name(line, filetype)
            break
        elif not line.startswith(line_token):
            if "name" not in entry:
                entry["name"] = get_field_name(line, filetype)
            break

        line = line[len(line_token):].strip()

        # generic parameter
        if line.startswith("@param"):
            entry["params"].append(parse_param(line, "param"))

        # typed parameter
        elif line.startswith("@number"):
            entry["params"].append(parse_param(line, "number"))
        elif line.startswith("@string"):
            entry["params"].append(parse_param(line, "string"))
        elif line.startswith("@table"):
            entry["params"].append(parse_param(line, "table"))
        elif line.startswith("@function"):
            entry["params"].append(parse_param(line, "function"))
        elif line.startswith("@bool"):
            entry["params"].append(parse_param(line, "boolean", original = "bool"))
        elif line.startswith("@vec2"):
            entry["params"].append(parse_param(line, "vec2"))
        elif line.startswith("@vec3"):
            entry["params"].append(parse_param(line, "vec3"))
        elif line.startswith("@vec4"):
            entry["params"].append(parse_param(line, "vec4"))

        # generic return
        elif line.startswith("@return"):
            line = line.replace("@return", "").strip()
            m = re.match("(\w*?) (.*)", line)
            if m:
                return_name = m.groups()[0]
                return_desc = m.groups()[1]
                entry["returns"].append({
                    "name": return_name,
                    "description": capitalize(return_desc)
                })
        # typed return
        elif line.startswith("@treturn"):
            line = line.replace("@treturn", "").strip()
            m = re.match("(\w+)\s*(\w+)\s*(.*)", line)
            if m:
                return_type = m.groups()[0]
                return_name = m.groups()[1]
                return_desc = m.groups()[2]
                entry["returns"].append({
                    "name": return_name,
                    "type": return_type,
                    "description": capitalize(return_desc)
                })
        # typed parameter
        elif line.startswith("@tparam"):
            line = line.replace("@tparam", "").strip()
            m = re.match("(\w+)\s*(\w+)\s*(.*)", line)
            if m:
                param_type = m.groups()[0]
                param_name = m.groups()[1]
                param_desc = m.groups()[2]
                entry["params"].append({
                    "name": param_name,
                    "type": param_type,
                    "description": capitalize(param_desc)
                })
        elif line.startswith("@field"):
            entry["field"] = True
            m = re.match("\@field (.*)", line)
            if m:
                entry["field_type"] = m.groups()[0]
                if not "name" in entry:
                    entry["name"] = m.groups()[0]
        elif line.startswith("@name"):
            line = line.replace("@name", "").strip()
            entry["name"] = line
        elif line.startswith("@usage"):
            entry["usage"] = line.replace("@usage", "")
        elif line.startswith("@"):
            m = re.match("\@(\w*?) (.*)", line)
            if m:
                tag = m.groups()[0]
                text = m.groups()[1]
                entry[tag] = text
            else:
                print("Found unknown tag: " + line)
        else:
            if entry.get("usage") is not None:
                entry["usage"] = entry["usage"] + line + "\n"
            else:
                entry["description"] = entry["description"] + line + " "

    has_params = len(entry["params"]) > 0
    if has_params:
        params = []
        for p in entry["params"]:
            params.append(p["name"])
        entry["params_string"] = ",".join(params)

    entry["has_params"] = has_params
    entry["has_returns"] = len(entry["returns"]) > 0
    entry["description"] = capitalize(entry["description"].strip())
    return entry


def process_lines(lines, filetype):
    print("Processing {}".format(filename))
    entries = []
    while len(lines) > 0:
        line = lines.pop(0).strip()
        if line.startswith(entry_start_token(filetype)):
            entry = process_entry(line, lines, filetype)
            entries.append(entry)
    return entries


groups_lookup = {}

directory = "imgui"

for filetype in ["lua", "cpp"]:

    pattern = "*." + filetype
    files = find_files(directory, pattern)
    for filename in files:
        with open(filename, encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
            file_description = None
            first_line = lines.pop(0).strip()
            if first_line.startswith(entry_start_token(filetype)):
                file_description = process_entry(first_line, lines, filetype)
            if not file_description:
                file_description = {}
            else:
                file_description["file_summary"] = file_description["summary"]
                file_description["file_description"] = file_description["description"]
                file_description["summary"] = None
                file_description["description"] = None
            
            file_entries = process_lines(lines, filetype)
            if file_entries:
                # components, events, info etc
                if "group" not in file_description:
                    group_name = os.path.dirname(filename).replace(directory + "/", "")
                    file_description["group"] = group_name
                if "module" not in file_description:
                    module = os.path.basename(filename).replace("." + filetype, "")
                    file_description["module"] = module
                file_description["entries"] = file_entries
                file_description["filename"] = filename

                group_name = file_description["group"]
                if group_name not in groups_lookup:
                    groups_lookup[group_name] = {
                        "group": group_name,
                        "files": []
                    }
                groups_lookup[group_name]["files"].append(file_description)


groups = []
for group_name in groups_lookup.keys():
    group = groups_lookup.get(group_name)
    groups.append(group)


api = { "groups": groups }
with open("api.json", "w", encoding='utf-8') as f:
    json.dump(api, f, indent=4, sort_keys=True)

render(api, "api_markdown.mtl", "api.md")
render(api, "api_script.mtl", os.path.join(directory, "api", directory + ".script_api"))
