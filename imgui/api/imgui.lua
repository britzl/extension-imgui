---@meta
---@diagnostic disable

--*****************************************************************************************************
--***** DRAG AND DROP *********************************************************************************
--*****************************************************************************************************

--- Begins a drag and drop source.
---@param flags DropFlags The flags for the drag and drop source.
---@return boolean result Whether the drag and drop source began successfully.
function imgui.begin_dragdrop_source(flags) end

--*****************************************************************************************************

--- Ends the drag and drop source.
function imgui.end_dragdrop_source() end

--*****************************************************************************************************

--- Begins a drag and drop target.
---@return boolean result Whether the drag and drop target began successfully.
function imgui.begin_dragdrop_target() end

--*****************************************************************************************************

--- Ends the drag and drop target.
function imgui.end_dragdrop_target() end

--*****************************************************************************************************

--- Sets the drag and drop payload.
---@param type string The type of the payload.
---@param payload string The payload data.
---@return boolean result Whether the payload was set successfully.
function imgui.set_dragdrop_payload(type, payload) end

--*****************************************************************************************************

--- Accepts the drag and drop payload.
---@param type string The type of the payload to accept.
---@param flags DropFlags|nil The flags for accepting the payload.
---@return string payload The accepted payload data.
function imgui.accept_dragdrop_payload(type, flags) end

--*****************************************************************************************************
--***** IMAGE *****************************************************************************************
--*****************************************************************************************************

--- Loads an image from a file.
---@param filename string The filename of the image.
---@return number imageid The ID of the loaded image.
function imgui.image_load(filename) end

--*****************************************************************************************************

--- Loads image data.
---@param imagename string The name of the image.
---@param image_data string The raw image data.
---@param image_data_len number The length of the image data.
---@return number imageid The ID of the loaded image.
function imgui.image_load_data(imagename, image_data, image_data_len) end

--*****************************************************************************************************

--- Gets an image by name.
---@param imagename string The name of the image.
---@return number imageid The ID of the image.
function imgui.image_get(imagename) end

--*****************************************************************************************************

--- Frees an image.
---@param imageid number The ID of the image to free.
function imgui.image_free(imageid) end

--*****************************************************************************************************

--- Adds an image.
---@param imageid number The ID of the image.
---@param pixel_width number The width of the image in pixels.
---@param pixel_height number The height of the image in pixels.
---@param uv0x number|nil The u-coordinate of the first UV point.
---@param uv0y number|nil The v-coordinate of the first UV point.
---@param uv1x number|nil The u-coordinate of the second UV point.
---@param uv1y number|nil The v-coordinate of the second UV point.
function imgui.image_add(imageid, pixel_width, pixel_height, uv0x, uv0y, uv1x, uv1y) end

--*****************************************************************************************************

--- Decodes a base64-encoded image.
---@param image_data_base64 string The base64-encoded image data.
---@param image_data_len number The length of the base64-encoded image data.
---@return string image_data_raw The raw image data.
function imgui.image_b64_decode(image_data_base64, image_data_len) end

--*****************************************************************************************************
--***** FONTS *****************************************************************************************
--*****************************************************************************************************

--- Adds a font from a TTF file.
---@param ttf_filename string The filename of the TTF file.
---@param fontsize number The size of the font.
---@param glyphranges number The glyph ranges for the font.
---@return number fontid The ID of the added font.
function imgui.font_add_ttf_file(ttf_filename, fontsize, glyphranges) end

--*****************************************************************************************************

--- Adds a font from TTF data.
---@param ttf_data string The TTF data.
---@param ttf_data_size number The size of the TTF data.
---@param fontsize number The size of the font.
---@param fontpixels number The pixels of the font.
---@param glyphranges number|nil The glyph ranges for the font.
---@return number fontid The ID of the added font.
function imgui.font_add_ttf_data(ttf_data, ttf_data_size, fontsize, fontpixels, glyphranges) end

--*****************************************************************************************************

--- Pushes a font onto the stack.
---@param fontid number The ID of the font to push.
function imgui.font_push(fontid) end

--*****************************************************************************************************

--- Pops the current font from the stack.
function imgui.font_pop() end

--*****************************************************************************************************

--- Scales a font.
---@param fontid number The ID of the font to scale.
---@param fontscale number The scale factor for the font.
function imgui.font_scale(fontid, fontscale) end

--*****************************************************************************************************

--- Gets the frame height.
---@return number height The height of the frame.
function imgui.get_frame_height() end

--*****************************************************************************************************
--***** WINDOW ****************************************************************************************
--*****************************************************************************************************

--- Sets the cursor position.
---@param x number The x-coordinate of the cursor position.
---@param y number The y-coordinate of the cursor position.
function imgui.set_cursor_pos(x, y) end

--*****************************************************************************************************

--- Sets the display size.
---@param width number The width of the display.
---@param height number The height of the display.
function imgui.set_display_size(width, height) end

--*****************************************************************************************************

--- Sets the next window size.
---@param width number The width of the window.
---@param height number The height of the window.
---@param cond number|nil Optional condition for setting the window size.
function imgui.set_next_window_size(width, height, cond) end

--*****************************************************************************************************

--- Sets the next window position.
---@param x number The x-coordinate of the window position.
---@param y number The y-coordinate of the window position.
---@param cond number|nil Optional condition for setting the window position.
function imgui.set_next_window_pos(x, y, cond) end

--*****************************************************************************************************

--- Gets the current window size.
---@return number width The width of the window.
---@return number height The height of the window.
function imgui.get_window_size() end

--*****************************************************************************************************

--- Gets the current window position.
---@return number x The x-coordinate of the window position.
---@return number y The y-coordinate of the window position.
function imgui.get_window_pos() end

--*****************************************************************************************************

--- Begins a new window.
---@param title string The title of the window.
---@param open boolean|nil Whether the window is open.
---@param flags WindowFlags|nil The flags for the window.
---@return boolean result Whether the window began successfully.
---@return boolean isopen Whether the window is open.
function imgui.begin_window(title, open, flags) end

--*****************************************************************************************************

--- Ends the current window.
function imgui.end_window() end

--*****************************************************************************************************

--- Checks if the window is focused.
---@param flags WindowFlags|nil The flags for checking focus.
---@return boolean focused Whether the window is focused.
function imgui.is_window_focused(flags) end

--*****************************************************************************************************

--- Gets the available content region.
---@return number w The width of the available content region.
---@return number h The height of the available content region.
function imgui.get_content_region_avail() end

--*****************************************************************************************************

--- Gets the maximum content region of the window.
---@return number x The x-coordinate of the content region max.
---@return number y The y-coordinate of the content region max.
function imgui.get_window_content_region_max() end

--*****************************************************************************************************

--- Begins a new child window.
---@param title string The title of the child window.
---@param width number The width of the child window.
---@param height number The height of the child window.
---@return boolean result Whether the child window began successfully.
function imgui.begin_child(title, width, height) end

--*****************************************************************************************************

--- Ends the current child window.
function imgui.end_child() end

--*****************************************************************************************************
--***** COMBO *****************************************************************************************
--*****************************************************************************************************

--- Begins a popup context item.
---@param id string The identifier of the popup.
---@param popup_flags PopupFlags|nil The flags for the popup.
---@return boolean result Whether the popup began successfully.
function imgui.begin_popup_context_item(id, popup_flags) end

--*****************************************************************************************************

--- Begins a popup.
---@param id string The identifier of the popup.
---@param window_flags WindowFlags The flags for the window.
---@return boolean result Whether the popup began successfully.
function imgui.begin_popup(id, window_flags) end

--*****************************************************************************************************

--- Begins a modal popup.
---@param name string The name of the popup.
---@param window_flags WindowFlags The flags for the window.
---@return boolean result Whether the popup began successfully.
function imgui.begin_popup_modal(name, window_flags) end

--*****************************************************************************************************

--- Opens a popup.
---@param id string The identifier of the popup.
---@param popup_flags PopupFlags The flags for the popup.
function imgui.open_popup(id, popup_flags) end

--*****************************************************************************************************

--- Closes the current popup.
function imgui.close_current_popup() end

--*****************************************************************************************************

--- Ends the current popup.
function imgui.end_popup() end

--*****************************************************************************************************
--***** COMBO *****************************************************************************************
--*****************************************************************************************************

--- Begins a combo box.
---@param label string The label of the combo box.
---@param preview string The preview text for the combo box.
---@return boolean result Whether the combo box began successfully.
function imgui.begin_combo(label, preview) end

--*****************************************************************************************************

--- Ends the current combo box.
function imgui.end_combo() end

--*****************************************************************************************************

--- Creates a combo box.
---@param label string The label of the combo box.
---@param current number The current item index.
---@param items table The items in the combo box.
---@return boolean result Whether the combo box was created successfully.
---@return number current The updated current item index.
function imgui.combo(label, current, items) end

--*****************************************************************************************************
--***** TABLE *****************************************************************************************
--*****************************************************************************************************

--- Begins a table.
---@param id string The identifier of the table.
---@param column number The number of columns.
---@param flags TableFlags|nil The flags for the table.
---@param outer_size_x number|nil The outer width of the table.
---@param outer_size_y number|nil The outer height of the table.
---@return boolean result Whether the table began successfully.
function imgui.begin_table(id, column, flags, outer_size_x, outer_size_y) end

--*****************************************************************************************************

--- Ends the current table.
function imgui.end_table() end

--*****************************************************************************************************

--- Creates a table headers row.
function imgui.table_headers_row() end

--*****************************************************************************************************

--- Sets up a table column.
---@param label string The label of the column.
---@param flags TableFlags|nil The flags for the column.
---@param init_width_or_weight number|nil The initial width or weight of the column.
function imgui.table_setup_column(label, flags, init_width_or_weight) end

--*****************************************************************************************************

--- Sets up scroll freeze for a table.
---@param columns number The number of columns to freeze.
---@param rows number The number of rows to freeze.
function imgui.table_setup_scroll_freeze(columns, rows) end

--*****************************************************************************************************

--- Sets the current table column index.
---@param column number The index of the column.
function imgui.table_set_column_index(column) end

--*****************************************************************************************************

--- Advances to the next table column.
function imgui.table_next_column() end

--*****************************************************************************************************

--- Advances to the next table row.
function imgui.table_next_row() end

--*****************************************************************************************************
--***** TREE NODE *************************************************************************************
--*****************************************************************************************************

--- Creates a tree node widget with the given text.
---@param text string The label of the tree node.
---@param TreeNode flags A number representing the status or attributes of the tree node.
function imgui.tree_node(text, flags)
end

--- Ends a tree node, balancing a previous call to tree_node.
function imgui.tree_pop()
end

--*****************************************************************************************************
--***** PUSH/POP ID ***********************************************************************************
--*****************************************************************************************************

--- Pushes a unique identifier onto the ID stack.
-- Used to avoid ID conflicts when multiple UI elements share the same label.
---@param text string A string that serves as the unique identifier.
function imgui.push_id(text)
end

--- Pops an identifier from the ID stack.
-- Balances a previous call to push_id.
function imgui.pop_id()
end

--*****************************************************************************************************
--***** TOOLTIP ***************************************************************************************
--*****************************************************************************************************

--- Begins displaying a tooltip.
-- Call this function before defining the content of the tooltip.
function imgui.begin_tooltip()
end

--- Ends a tooltip, balancing a previous call to begin_tooltip.
function imgui.end_tooltip()
end

--*****************************************************************************************************
--***** TAB BAR ***************************************************************************************
--*****************************************************************************************************

--- Begins a tab bar with a specific ID.
---@param id string A string representing the ID of the tab bar.
---@return boolean result A boolean indicating if the tab bar was successfully created.
function imgui.begin_tab_bar(id)
end

--- Ends a tab bar, balancing a previous call to begin_tab_bar.
function imgui.end_tab_bar()
end

--- Begins a tab item within a tab bar.
---@param label string A string representing the label of the tab item.
---@param open boolean|nil A boolean indicating whether the tab item is open.
---@param flags TabItem|nil A number representing additional options or styles for the tab item.
---@return boolean result A boolean indicating if the tab item was successfully created.
---@return boolean open A boolean indicating whether the tab item remains open.
function imgui.begin_tab_item(label, open, flags)
end

--*****************************************************************************************************
--***** COMPONENTS ************************************************************************************
--*****************************************************************************************************

--- Displays text in the UI.
---@param text string A string of the text to display.
function imgui.text(text)
end

--- Displays colored text in the UI.
---@param text string A string of the text to display.
---@param Red number A number for the red component of the text color (0 to 255).
---@param Green number A number for the green component of the text color (0 to 255).
---@param Blue number A number for the blue component of the text color (0 to 255).
---@param Alpha number A number for the alpha component of the text color (0 to 255).
function imgui.text_colored(text, Red, Green, Blue, Alpha)
end

--- Displays an input text box.
---@param label string A string label for the input field.
---@param text string A string that holds the current text value.
---@param flags InputFlags A number for additional options affecting input behavior.
---@return boolean changed A boolean indicating if the text was modified by the user.
function imgui.input_text(label, text, flags)
end

--- Displays an input field for integer values.
---@param label string A string label for the input field.
---@param value number A number representing the initial integer value.
---@return boolean changed A boolean indicating if the value was modified by the user.
function imgui.input_int(label, value)
end

--- Displays an input field for floating-point values.
---@param label string A string label for the input field.
---@param float number A number representing the initial floating-point value.
---@param step number|nil A number for the increment step when adjusting the value.
---@param step_fast number|nil A number for the larger increment step for fast adjustment.
---@param float_precision number|nil A number indicating the precision of the float value.
---@return number new_float A number representing the new floating-point value after adjustment.
function imgui.input_float(label, float, step, step_fast, float_precision)
end

--- Displays an input field for three floating-point values.
-- Useful for inputting 3D vectors.
---@param label string A string label for the input field.
---@param v1 number A number representing the first float value.
---@param v2 number A number representing the second float value.
---@param v3 number A number representing the third float value.
---@return boolean changed A boolean indicating if any value was modified by the user.
---@return number v1 The updated first float value.
---@return number v2 The updated second float value.
---@return number v3 The updated third float value.
function imgui.input_float3(label, v1, v2, v3)
end

--- Displays an input field for four floating-point values.
-- Useful for inputting 4D vectors or quaternions.
---@param label string A string label for the input field.
---@param v1 number A number representing the first float value.
---@param v2 number A number representing the second float value.
---@param v3 number A number representing the third float value.
---@param v4 number A number representing the fourth float value.
---@return boolean changed A boolean indicating if any value was modified by the user.
---@return number v1 The updated first float value.
---@return number v2 The updated second float value.
---@return number v3 The updated third float value.
---@return number v4 The updated fourth float value.
function imgui.input_float4(label, v1, v2, v3, v4)
end

--- Displays an input field for double-precision floating-point values.
---@param label string A string label for the input field.
---@param double number A number representing the initial double value.
---@param step number A number for the increment step when adjusting the value.
---@param step_fast number A number for the larger increment step for fast adjustment.
---@param double_precision number A number indicating the precision of the double value.
---@return number new_double A number representing the new double value after adjustment.
function imgui.input_double(label, double, step, step_fast, double_precision)
end

--- Displays a slider for floating-point values.
---@param label string A string label for the slider.
---@param float number A number representing the initial slider value.
---@param min number A number indicating the minimum slider value.
---@param max number A number indicating the maximum slider value.
---@param float_precision number|nil A number indicating the precision of the slider value.
---@return number new_float A number representing the new slider value.
function imgui.slider_float(label, float, min, max, float_precision)
end

--- Displays a selectable item.
---@param text string A string label for the selectable item.
---@param selected boolean A boolean indicating whether the item is initially selected.
---@param flags Selectable|nil A number representing additional options or styles for the item.
---@return boolean selected A boolean indicating whether the item is selected after interaction.
function imgui.selectable(text, selected, flags)
end

--- Displays a button.
---@param text string A string label for the button.
---@return boolean pushed A boolean indicating if the button was clicked.
function imgui.button(text)
end

--- Displays a button with an image.
---@param imageid number A number representing the image ID.
---@param width number(opt) A number for the optional width of the button.
---@param height number(opt) A number for the optional height of the button.
---@return boolean pushed A boolean indicating if the button was clicked.
function imgui.button_image(imageid, width, height)
end

--- Displays a checkbox.
---@param text string A string label for the checkbox.
---@param checked boolean A boolean indicating whether the checkbox is initially checked.
---@return boolean changed A boolean indicating if the checked state was changed by the user.
---@return boolean pushed A boolean indicating if the checkbox was clicked.
function imgui.checkbox(text, checked)
end

--- Displays a radio button.
---@param text string A string label for the radio button.
---@param checked boolean A boolean indicating whether the radio button is initially checked.
---@return boolean changed A boolean indicating if the checked state was changed by the user.
---@return boolean pushed A boolean indicating if the radio button was clicked.
function imgui.radio_button(text, checked)
end

--*****************************************************************************************************
--***** MENU BAR **************************************************************************************
--*****************************************************************************************************

--- Begins a menu bar. 
-- Should be called before adding menu items.
---@return boolean result A boolean indicating whether the menu bar was successfully created.
function imgui.begin_menu_bar()
end

--- Ends a menu bar, balancing a previous call to begin_menu_bar.
function imgui.end_menu_bar()
end

--- Begins the main menu bar.
-- Should be called before adding top-level menu items.
---@return boolean result A boolean indicating whether the main menu bar was successfully created.
function imgui.begin_main_menu_bar()
end

--- Ends the main menu bar, balancing a previous call to begin_main_menu_bar.
function imgui.end_main_menu_bar()
end

--- Begins a menu with a specific label.
-- Allows nesting of menu items.
---@param label string The label of the menu.
---@param enabled boolean|nil An optional boolean to enable or disable the menu. Defaults to true.
---@return boolean result A boolean indicating whether the menu was successfully created.
function imgui.begin_menu(label, enabled)
end

--- Ends a menu, balancing a previous call to begin_menu.
function imgui.end_menu()
end

--- Creates a menu item with optional shortcut and selection state.
-- Menu items can be nested within a menu.
---@param label string The label of the menu item.
---@param shortcut string|nil An optional string indicating the keyboard shortcut for the item.
---@param selected boolean|nil An optional boolean indicating whether the item is initially selected. Defaults to false.
---@param enabled boolean|nil An optional boolean to enable or disable the menu item. Defaults to true.
---@return boolean changed A boolean indicating if the menu item's state was changed.
---@return boolean selected A boolean indicating whether the item is selected after interaction.
function imgui.menu_item(label, shortcut, selected, enabled)
end

--*****************************************************************************************************
--***** LAYOUT ****************************************************************************************
--*****************************************************************************************************

--- Positions the next widget on the same line with an optional horizontal offset.
---@param offset number|nil The horizontal offset from the current position.
function imgui.same_line(offset)
end

--- Moves the cursor to the next line.
-- Inserts a new line in the layout.
function imgui.new_line()
end

--- Adds a bullet point marker to the current line.
-- Useful for creating bullet lists.
function imgui.bullet()
end

--- Increases the indentation level for subsequent widgets.
function imgui.indent()
end

--- Decreases the indentation level for subsequent widgets.
-- Balances a previous call to indent.
function imgui.unindent()
end

--- Inserts a vertical spacing between widgets.
function imgui.spacing()
end

--- Inserts a horizontal line separator.
function imgui.separator()
end

--- Retrieves the current cursor position on the screen.
---@return number x The x-coordinate of the cursor.
---@return number y The y-coordinate of the cursor.
function imgui.get_cursor_screen_pos()
end

--*****************************************************************************************************
--***** PRIMITIVES ************************************************************************************
--*****************************************************************************************************

--- Draws a line between two points with specified color and thickness.
---@param x1 number The x-coordinate of the starting point.
---@param y1 number The y-coordinate of the starting point.
---@param x2 number The x-coordinate of the ending point.
---@param y2 number The y-coordinate of the ending point.
---@param r number The red component of the line color (0 to 255).
---@param g number The green component of the line color (0 to 255).
---@param b number The blue component of the line color (0 to 255).
---@param a number The alpha component of the line color (0 to 255).
---@param thickness number The thickness of the line.
function imgui.add_line(x1, y1, x2, y2, r, g, b, a, thickness)
end

--- Draws a rectangle with specified color and thickness.
-- The rectangle is defined by its top-left and bottom-right corners.
---@param x1 number The x-coordinate of the top-left corner.
---@param y1 number The y-coordinate of the top-left corner.
---@param x2 number The x-coordinate of the bottom-right corner.
---@param y2 number The y-coordinate of the bottom-right corner.
---@param r number The red component of the rectangle's border color (0 to 255).
---@param g number The green component of the rectangle's border color (0 to 255).
---@param b number The blue component of the rectangle's border color (0 to 255).
---@param a number The alpha component of the rectangle's border color (0 to 255).
---@param thickness number The thickness of the rectangle's border.
function imgui.add_rect(x1, y1, x2, y2, r, g, b, a, thickness)
end

--*****************************************************************************************************
--***** PLOTS *****************************************************************************************
--*****************************************************************************************************

--- Plots a series of lines based on provided data.
-- Used for creating line charts or graphs.
---@param label string The label for the plot.
---@param valueoffset number The offset in the value array to start plotting from.
---@param width number The width of the plot area.
---@param height number The height of the plot area.
---@param plotdata table A table containing the data points to plot.
function imgui.plot_lines(label, valueoffset, width, height, plotdata)
end

--- Plots a histogram based on provided data.
-- Used for creating bar charts or histograms.
---@param label string The label for the histogram.
---@param valueoffset number The offset in the value array to start plotting from.
---@param width number The width of the histogram area.
---@param height number The height of the histogram area.
---@param plotdata table A table containing the data points to plot.
function imgui.plot_histogram(label, valueoffset, width, height, plotdata)
end

--- Retrieves the size of the given text in pixels.
---@return number textpixelsize The width of the text in pixels.
function imgui.text_getsize()
end

--*****************************************************************************************************
--***** DRAWING ***************************************************************************************
--*****************************************************************************************************

--- Draws a line between two points with a specified color.
---@param x1 number The x-coordinate of the starting point.
---@param y1 number The y-coordinate of the starting point.
---@param x2 number The x-coordinate of the ending point.
---@param y2 number The y-coordinate of the ending point.
---@param color number The color of the line, represented as a packed integer.
function imgui.draw_line(x1, y1, x2, y2, color)
end

--- Draws a rectangle outline at a specified position with a specified color.
---@param x number The x-coordinate of the top-left corner.
---@param y number The y-coordinate of the top-left corner.
---@param width number The width of the rectangle.
---@param height number The height of the rectangle.
---@param color number The color of the rectangle, represented as a packed integer.
function imgui.draw_rect(x, y, width, height, color)
end

--- Draws a filled rectangle at a specified position with a specified color.
---@param x number The x-coordinate of the top-left corner.
---@param y number The y-coordinate of the top-left corner.
---@param width number The width of the rectangle.
---@param height number The height of the rectangle.
---@param color number The color of the rectangle, represented as a packed integer.
function imgui.draw_rect_filled(x, y, width, height, color)
end

--- Draws a progress bar with a specified size and progress value.
---@param progress number The progress value (0.0 to 1.0) representing the percentage of the bar to fill.
---@param sizex number The width of the progress bar.
---@param sizey number The height of the progress bar.
function imgui.draw_progress(progress, sizex, sizey)
end

--- Enables or disables rendering.
-- Used to control whether rendering should be done for the current frame.
---@param enabled boolean A boolean value indicating whether rendering should be enabled.
function imgui.set_rendering_enabled(enabled)
end

--*****************************************************************************************************
--***** INPUT *****************************************************************************************
--*****************************************************************************************************

--- Sets the current mouse input state.
-- Includes mouse position, button states, and wheel movement.
---@param x number The x-coordinate of the mouse cursor.
---@param y number The y-coordinate of the mouse cursor.
---@param mouse1 number The state of the first mouse button (0 or 1).
---@param mouse2 number The state of the second mouse button (0 or 1).
---@param mouse3 number The state of the third mouse button (0 or 1).
---@param mousewheel number The movement of the mouse wheel.
function imgui.set_mouse_input(x, y, mouse1, mouse2, mouse3, mousewheel)
end

--- Sets the state of a specific mouse button.
-- Use constants such as imgui.MOUSEBUTTON_LEFT, imgui.MOUSEBUTTON_MIDDLE, or imgui.MOUSEBUTTON_RIGHT.
---@param index MouseButton The index of the mouse button.
---@param state number The state of the mouse button (0 for released, 1 for pressed).
function imgui.set_mouse_button(index, state)
end

--- Sets the mouse wheel movement state.
---@param mousewheel number The movement of the mouse wheel.
function imgui.set_mouse_wheel(mousewheel)
end

--- Sets the current mouse position.
---@param x number The x-coordinate of the mouse cursor.
---@param y number The y-coordinate of the mouse cursor.
function imgui.set_mouse_pos(x, y)
end

--- Sets the state of a specific key.
---@param keyid Key The ID of the key to set.
---@param state boolean A boolean value indicating whether the key is pressed.
function imgui.set_key_down(keyid, state)
end

--- Sets the control key modifier state.
-- Controls whether the CTRL key is considered pressed.
---@param state boolean A boolean value indicating whether the CTRL key is pressed.
function imgui.set_key_modifier_ctrl(state)
end

--- Sets the shift key modifier state.
-- Controls whether the SHIFT key is considered pressed.
---@param state boolean A boolean value indicating whether the SHIFT key is pressed.
function imgui.set_key_modifier_shift(state)
end

--- Sets the alt key modifier state.
-- Controls whether the ALT key is considered pressed.
---@param state boolean A boolean value indicating whether the ALT key is pressed.
function imgui.set_key_modifier_alt(state)
end

--- Sets the super key modifier state.
-- Controls whether the SUPER key is considered pressed.
---@param state boolean A boolean value indicating whether the SUPER key is pressed.
function imgui.set_key_modifier_super(state)
end

--- Adds a single input character to the input buffer.
---@param character string A single character to be added to the input buffer.
function imgui.add_input_character(character)
end

--- Adds multiple input characters to the input buffer.
---@param characters string A string of characters to be added to the input buffer.
function imgui.add_input_characters(characters)
end

--- Checks if a mouse button was double-clicked.
---@param button number The index of the mouse button to check.
---@return boolean clicked A boolean indicating if the mouse button was double-clicked.
function imgui.is_mouse_double_clicked(button)
end

--- Checks if a mouse button was clicked.
---@param button number The index of the mouse button to check.
---@return boolean clicked A boolean indicating if the mouse button was clicked.
function imgui.is_mouse_clicked(button)
end

--- Checks if the current item is active.
---@return boolean active A boolean indicating if the current item is active.
function imgui.is_item_active()
end

--- Checks if the current item is focused.
---@return boolean focused A boolean indicating if the current item is focused.
function imgui.is_item_focused()
end

--- Checks if the current item was clicked.
---@param button number The index of the mouse button to check.
---@return boolean clicked A boolean indicating if the current item was clicked.
function imgui.is_item_clicked(button)
end

--- Checks if the current item was double-clicked.
---@param button number The index of the mouse button to check.
---@return boolean clicked A boolean indicating if the current item was double-clicked.
function imgui.is_item_double_clicked(button)
end

--- Checks if the current item is hovered.
---@return boolean hovered A boolean indicating if the current item is hovered.
function imgui.is_item_hovered()
end

--- Gets the maximum x and y coordinates of the current item's rectangle.
---@return number x The x-coordinate of the maximum point.
---@return number y The y-coordinate of the maximum point.
function imgui.get_item_rect_max()
end

--- Sets the keyboard focus to the current widget.
-- The focus can be offset by a specified amount.
---@param offset number The offset from the current widget to set focus.
function imgui.set_keyboard_focus_here(offset)
end

--*****************************************************************************************************
--***** STYLE *****************************************************************************************
--*****************************************************************************************************

--- @class Style
--- Configuration for ImGui styling
--- Border size for child windows
--- @field ChildBorderSize number
--- Rounding radius for popups
--- @field PopupRounding number
--- Border size for popups
--- @field PopupBorderSize number
--- Padding within a framed region (x, y)
--- @field FramePadding vector3
--- Rounding radius for frames
--- @field FrameRounding number
--- Border size for frames
--- @field FrameBorderSize number
--- Spacing between items
--- @field ItemSpacing vector3
--- Inner spacing between elements of an item
--- @field ItemInnerSpacing vector3
--- Padding inside a table cell
--- @field CellPadding vector3
--- Extra padding for touch inputs
--- @field TouchExtraPadding vector3
--- Indentation spacing for tree nodes
--- @field IndentSpacing number
--- Global alpha (transparency)
--- @field Alpha number
--- Width of the vertical scrollbar
--- @field ScrollbarSize number
--- Rounding radius for scrollbars
--- @field ScrollbarRounding number
--- Minimum size for grab areas
--- @field GrabMinSize number
--- Rounding radius for grab areas
--- @field GrabRounding number
--- Deadzone for log sliders
--- @field LogSliderDeadzone number
--- Rounding radius for tabs
--- @field TabRounding number
--- Border size for tabs
--- @field TabBorderSize number
--- Minimum width for the close button on tabs
--- @field TabMinWidthForCloseButton number
--- Position of color button
--- @field ColorButtonPosition number
--- Text alignment for buttons (x, y)
--- @field ButtonTextAlign vector3
--- Text alignment for selectable items (x, y)
--- @field SelectableTextAlign vector3
--- Padding for display windows (x, y)
--- @field DisplayWindowPadding vector3
--- Safe area padding for display windows (x, y)
--- @field DisplaySafeAreaPadding vector3
--- Scale for mouse cursor
--- @field MouseCursorScale number
--- Maximum allowed error for circle segments
--- @field CircleSegmentMaxError number
--- Minimum spacing between columns
--- @field ColumnsMinSpacing number
--- Enable anti-aliasing for lines
--- @field AntiAliasedLines boolean
--- Enable texture-based anti-aliasing for lines
--- @field AntiAliasedLinesUseTex boolean
--- Enable anti-aliasing for filled shapes
--- @field AntiAliasedFill boolean
--- Tolerance for curve tessellation
--- @field CurveTessellationTol number
--- Padding for window content (x, y)
--- @field WindowPadding vector3
--- Rounding radius for windows
--- @field WindowRounding number
--- Border size for windows
--- @field WindowBorderSize number
--- Minimum size for windows (width, height)
--- @field WindowMinSize vector3
--- Alignment for window titles (x, y)
--- @field WindowTitleAlign vector3
--- Position for window menu buttons
--- @field WindowMenuButtonPosition number
--- Rounding radius for child windows
--- @field ChildRounding number

--- Sets the style for the UI.
-- This includes colors, sizes, and other style parameters.
---@param style Style A table containing style attributes and values.
function imgui.set_style(style)
end

--- Gets the current style settings.
---@return Style style A table containing the current style attributes and values.
function imgui.get_style()
end

--- Sets the window rounding style.
-- This controls how rounded the corners of the windows are.
---@param rounding number The amount of rounding for window corners.
function imgui.set_style_window_rounding(rounding)
end

--- Sets the border size of the window.
---@param bordersize number The size of the window borders.
function imgui.set_style_window_bordersize(bordersize)
end

--- Sets the border size for child windows.
---@param bordersize number The size of the child window borders.
function imgui.set_style_child_bordersize(bordersize)
end

--- Sets the frame rounding style.
-- This controls how rounded the corners of frames are.
---@param rounding number The amount of rounding for frame corners.
function imgui.set_style_frame_rounding(rounding)
end

--- Sets the tab rounding style.
-- This controls how rounded the corners of tabs are.
---@param rounding number The amount of rounding for tab corners.
function imgui.set_style_tab_rounding(rounding)
end

--- Sets the scrollbar rounding style.
-- This controls how rounded the corners of scrollbars are.
---@param rounding number The amount of rounding for scrollbar corners.
function imgui.set_style_scrollbar_rounding(rounding)
end

--- Sets the size of the scrollbar.
---@param size number The size of the scrollbar.
function imgui.set_style_scrollbar_size(size)
end

--- Sets the color for a specific style element.
---@param style number The ID of the style element to color.
---@param r number The red component of the color (0 to 255).
---@param g number The green component of the color (0 to 255).
---@param b number The blue component

--- Sets the color for a specific style element.
-- This affects UI elements such as windows, frames, and text.
---@param style number The ID of the style element to color.
---@param r number The red component of the color (0 to 255).
---@param g number The green component of the color (0 to 255).
---@param b number The blue component of the color (0 to 255).
---@param a number The alpha (transparency) component of the color (0 to 255).
function imgui.set_style_color(style, r, g, b, a)
end

--- Pushes a new style color onto the stack.
-- Used to temporarily change colors, which can later be reverted.
---@param color number The ID of the style element to color.
---@param r number The red component of the color (0 to 255).
---@param g number The green component of the color (0 to 255).
---@param b number The blue component of the color (0 to 255).
---@param a number The alpha (transparency) component of the color (0 to 255).
function imgui.push_style_color(color, r, g, b, a)
end

--- Pops style colors off the stack, reverting to the previous style.
-- This restores the color state before the corresponding push.
---@param count number The number of colors to pop off the stack.
function imgui.pop_style_color(count)
end

--- Gets the item spacing style.
-- Returns the spacing between UI elements.
---@return number x The horizontal spacing between items.
---@return number y The vertical spacing between items.
function imgui.get_style_item_spacing()
end

--- Sets the font scale for the current window.
-- Used to adjust the font size relative to the default size.
---@param fontscale number The scale factor for the font (1.0 is default size).
function imgui.set_window_font_scale(fontscale)
end

--- Sets the global font scale.
-- This affects all text in the UI, adjusting size globally.
---@param fontscale number The scale factor for the global font (1.0 is default size).
function imgui.set_global_font_scale(fontscale)
end

--- Scales all sizes in the UI by a given factor.
-- This includes elements such as padding, margins, and spacing.
---@param scale number The factor by which to scale all UI sizes (1.0 is default).
function imgui.scale_all_sizes(scale)
end

--*****************************************************************************************************
--***** ITEM WIDTH ************************************************************************************
--*****************************************************************************************************

--- Pushes a new item width onto the stack.
-- This sets the width for the next item and can be reverted using `pop_item_width`.
---@param width number The width to set for the next item.
function imgui.push_item_width(width)
end

--- Pops the last item width off the stack.
-- Reverts to the previous item width setting.
function imgui.pop_item_width()
end

--- Sets the width for the next item.
-- Affects the next UI item rendered, without affecting others.
---@param width number The width to set for the next item.
function imgui.set_next_item_width(width)
end

--- Calculates the width of the current item.
-- Useful for adjusting layouts dynamically based on item size.
---@return number width The width of the current item.
function imgui.calc_item_width()
end

--*****************************************************************************************************
--***** NAVIGATION ************************************************************************************
--*****************************************************************************************************

--- Sets the vertical scroll position to bring the current item into view.
-- Centers the item vertically based on the specified ratio.
---@param center_y_ratio number The ratio to center the item (0.5 centers, 0 aligns top).
function imgui.set_scroll_here_y(center_y_ratio)
end

--*****************************************************************************************************
--***** CAPTURE ***************************************************************************************
--*****************************************************************************************************

--- Checks if mouse input is required by the UI.
-- Indicates whether the UI wants to capture mouse events.
---@return boolean capture True if the UI wants mouse input.
function imgui.want_mouse_input()
end

--- Checks if keyboard input is required by the UI.
-- Indicates whether the UI wants to capture keyboard events.
---@return boolean capture True if the UI wants keyboard input.
function imgui.want_keyboard_input()
end

--- Checks if text input is required by the UI.
-- Indicates whether the UI wants to capture text input events.
---@return boolean capture True if the UI wants text input.
function imgui.want_text_input()
end

--*****************************************************************************************************
--***** CONFIG ****************************************************************************************
--*****************************************************************************************************

--- Resets all configuration settings to their default values.
-- Useful for restoring the initial state of the UI configuration.
function imgui.set_defaults()
end

--- Sets the filename for storing configuration settings.
-- Changes the default location where UI settings are saved.
---@param filename string|nil The name of the file to use for configuration storage.
function imgui.set_ini_filename(filename)
end

--*****************************************************************************************************
--***** MOUSE BUTTON ENUM *****************************************************************************
--*****************************************************************************************************

---@enum MouseButton
imgui = {
    MOUSEBUTTON_LEFT = 1,
    MOUSEBUTTON_RIGHT = 2,
    MOUSEBUTTON_MIDDLE = 3
}

--*****************************************************************************************************
--***** SELECTABLE ENUM *******************************************************************************
--*****************************************************************************************************

---@enum Selectable
imgui = {
    SELECTABLE_DONT_CLOSE_POPUPS = 1,
    SELECTABLE_SPAN_ALL_COLUMNS = 2,
    SELECTABLE_ALLOW_DOUBLE_CLICK = 3,
    SELECTABLE_DISABLED = 4,
    SELECTABLE_ALLOW_ITEM_OVERLAP = 5
}

--*****************************************************************************************************
--***** TAB ITEM ENUM *********************************************************************************
--*****************************************************************************************************

---@enum TabItem
imgui = {
    TABITEM_UNSAVED_DOCUMENT = 1,
    TABITEM_SET_SELECTED = 2,
    TABITEM_NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON = 3,
    TABITEM_NO_PUSH_ID = 4,
    TABITEM_NO_TOOLTIP = 5,
    TABITEM_NO_REORDER = 6,
    TABITEM_LEADING = 7,
    TABITEM_TRAILING = 8
}

--*****************************************************************************************************
--***** FOCUSED ENUM **********************************************************************************
--*****************************************************************************************************

---@enum Focused
imgui = {
    FOCUSED_CHILD_WINDOWS = 1,
    FOCUSED_ROOT_WINDOW = 2,
    FOCUSED_ANY_WINDOW = 3,
    FOCUSED_ROOT_AND_CHILD_WINDOWS = 4
}

--*****************************************************************************************************
--***** TREE NODE ENUM ********************************************************************************
--*****************************************************************************************************

---@enum TreeNode
imgui = {
    TREENODE_SELECTED = 1,
    TREENODE_FRAMED = 2,
    TREENODE_ALLOW_ITEM_OVERLAP = 3,
    TREENODE_NO_TREE_PUSH_ON_OPEN = 4,
    TREENODE_NO_AUTO_OPEN_ON_LOG = 5,
    TREENODE_DEFAULT_OPEN = 6,
    TREENODE_OPEN_ON_DOUBLE_CLICK = 7,
    TREENODE_OPEN_ON_ARROW = 8,
    TREENODE_LEAF = 9,
    TREENODE_BULLET = 10,
    TREENODE_FRAME_PADDING = 11,
    TREENODE_SPAN_AVAILABLE_WIDTH = 12,
    TREENODE_SPAN_FULL_WIDTH = 13,
    TREENODE_NAV_LEFT_JUMPS_BACK_HERE = 14
}

--*****************************************************************************************************
--***** COLOR ENUM ************************************************************************************
--*****************************************************************************************************

---@enum ImGuiCol
imgui = {
    ImGuiCol_Text = 1,
    ImGuiCol_TextDisabled = 2,
    ImGuiCol_WindowBg = 3,
    ImGuiCol_ChildBg = 4,
    ImGuiCol_PopupBg = 5,
    ImGuiCol_Border = 6,
    ImGuiCol_BorderShadow = 7,
    ImGuiCol_FrameBg = 8,
    ImGuiCol_FrameBgHovered = 9,
    ImGuiCol_FrameBgActive = 10,
    ImGuiCol_TitleBg = 11,
    ImGuiCol_TitleBgActive = 12,
    ImGuiCol_TitleBgCollapsed = 13,
    ImGuiCol_MenuBarBg = 14,
    ImGuiCol_ScrollbarBg = 15,
    ImGuiCol_ScrollbarGrab = 16,
    ImGuiCol_ScrollbarGrabHovered = 17,
    ImGuiCol_ScrollbarGrabActive = 18,
    ImGuiCol_CheckMark = 19,
    ImGuiCol_SliderGrab = 20,
    ImGuiCol_SliderGrabActive = 21,
    ImGuiCol_Button = 22,
    ImGuiCol_ButtonHovered = 23,
    ImGuiCol_ButtonActive = 24,
    ImGuiCol_Header = 25,
    ImGuiCol_HeaderHovered = 26,
    ImGuiCol_HeaderActive = 27,
    ImGuiCol_Separator = 28,
    ImGuiCol_SeparatorHovered = 29,
    ImGuiCol_SeparatorActive = 30,
    ImGuiCol_ResizeGrip = 31,
    ImGuiCol_ResizeGripHovered = 32,
    ImGuiCol_ResizeGripActive = 33,
    ImGuiCol_Tab = 34,
    ImGuiCol_TabHovered = 35,
    ImGuiCol_TabActive = 36,
    ImGuiCol_TabUnfocused = 37,
    ImGuiCol_TabUnfocusedActive = 38,
    ImGuiCol_PlotLines = 39,
    ImGuiCol_PlotLinesHovered = 40,
    ImGuiCol_PlotHistogram = 41,
    ImGuiCol_PlotHistogramHovered = 42,
    ImGuiCol_TableHeaderBg = 43,
    ImGuiCol_TableBorderStrong = 44,
    ImGuiCol_TableBorderLight = 45,
    ImGuiCol_TableRowBg = 46,
    ImGuiCol_TableRowBgAlt = 47,
    ImGuiCol_TextSelectedBg = 48,
    ImGuiCol_DragDropTarget = 49,
    ImGuiCol_NavHighlight = 50,
    ImGuiCol_NavWindowingHighlight = 51,
    ImGuiCol_NavWindowingDimBg = 52,
    ImGuiCol_ModalWindowDimBg = 53
}

--*****************************************************************************************************
--***** TABLE COLUMN ENUM *****************************************************************************
--*****************************************************************************************************

---@enum TableColumn
imgui = {
    TABLECOLUMN_NONE = 1,
    TABLECOLUMN_DEFAULTHIDE = 2,
    TABLECOLUMN_DEFAULTSORT = 3,
    TABLECOLUMN_WIDTHSTRETCH = 4,
    TABLECOLUMN_WIDTHFIXED = 5,
    TABLECOLUMN_NORESIZE = 6,
    TABLECOLUMN_NOREORDER = 7,
    TABLECOLUMN_NOHIDE = 8,
    TABLECOLUMN_NOCLIP = 9,
    TABLECOLUMN_NOSORT = 10,
    TABLECOLUMN_NOSORTASCENDING = 11,
    TABLECOLUMN_NOSORTDESCENDING = 12,
    TABLECOLUMN_NOHEADERWIDTH = 13,
    TABLECOLUMN_PREFERSORTASCENDING = 14,
    TABLECOLUMN_PREFERSORTDESCENDING = 15,
    TABLECOLUMN_INDENTENABLE = 16,
    TABLECOLUMN_INDENTDISABLE = 17
}

--*****************************************************************************************************
--***** WINDOW FLAGS ENUM *****************************************************************************
--*****************************************************************************************************

---@enum WindowFlags
imgui = {
    WINDOWFLAGS_NONE = 1,
    WINDOWFLAGS_NOTITLEBAR = 2,
    WINDOWFLAGS_NORESIZE = 3,
    WINDOWFLAGS_NOMOVE = 4,
    WINDOWFLAGS_NOSCROLLBAR = 5,
    WINDOWFLAGS_NOSCROLLWITHMOUSE = 6,
    WINDOWFLAGS_NOCOLLAPSE = 7,
    WINDOWFLAGS_ALWAYSAUTORESIZE = 8,
    WINDOWFLAGS_NOBACKGROUND = 9,
    WINDOWFLAGS_NOSAVEDSETTINGS = 10,
    WINDOWFLAGS_NOMOUSEINPUTS = 11,
    WINDOWFLAGS_MENUBAR = 12,
    WINDOWFLAGS_HORIZONTALSCROLLBAR = 13,
    WINDOWFLAGS_NOFOCUSONAPPEARING = 14,
    WINDOWFLAGS_NOBRINGTOFRONTONFOCUS = 15,
    WINDOWFLAGS_ALWAYSVERTICALSCROLLBAR = 16,
    WINDOWFLAGS_ALWAYSHORIZONTALSCROLLBAR = 17,
    WINDOWFLAGS_ALWAYSUSEWINDOWPADDING = 18,
    WINDOWFLAGS_NONAVINPUTS = 19,
    WINDOWFLAGS_NONAVFOCUS = 20,
    WINDOWFLAGS_UNSAVEDDOCUMENT = 21,
    WINDOWFLAGS_NONAV = 22,
    WINDOWFLAGS_NODECORATION = 23,
    WINDOWFLAGS_NOINPUTS = 24
}

--*****************************************************************************************************
--***** POPUP FLAGS ENUM *****************************************************************************
--*****************************************************************************************************

---@enum PopupFlags
imgui = {
    POPUPFLAGS_NONE = 1,
    POPUPFLAGS_MOUSEBUTTONLEFT = 2,
    POPUPFLAGS_MOUSEBUTTONRIGHT = 3,
    POPUPFLAGS_MOUSEBUTTONMIDDLE = 4,
    POPUPFLAGS_MOUSEBUTTONMASK = 5,
    POPUPFLAGS_MOUSEBUTTONDEFAULT = 6,
    POPUPFLAGS_NOOPENOVEREXISTINGPOPUP = 7,
    POPUPFLAGS_NOOPENOVERITEMS = 8,
    POPUPFLAGS_ANYPOPUPID = 9,
    POPUPFLAGS_ANYPOPUPLEVEL = 10,
    POPUPFLAGS_ANYPOPUP = 11
}

--*****************************************************************************************************
--***** DROP FLAGS ENUM ******************************************************************************
--*****************************************************************************************************

---@enum DropFlags
imgui = {
    DROPFLAGS_NONE = 1,
    DROPFLAGS_SOURCENOPREVIEWTOOLTIP = 2,
    DROPFLAGS_SOURCENODISABLEHOVER = 3,
    DROPFLAGS_SOURCENOHOLDTOOPENOTHERS = 4,
    DROPFLAGS_SOURCEALLOWNULLID = 5,
    DROPFLAGS_SOURCEEXTERN = 6,
    DROPFLAGS_SOURCEAUTOEXPIREPAYLOAD = 7,
    DROPFLAGS_ACCEPTBEFOREDELIVERY = 8,
    DROPFLAGS_ACCEPTNODRAWDEFAULTRECT = 9,
    DROPFLAGS_ACCEPTNOPREVIEWTOOLTIP = 10,
    DROPFLAGS_ACCEPTPEEKONLY = 11
}

--*****************************************************************************************************
--***** TABLE ENUM ************************************************************************************
--*****************************************************************************************************

---@enum TableFlags
imgui = {
    TABLE_NONE = 0,
    TABLE_RESIZABLE = 1,
    TABLE_REORDERABLE = 2,
    TABLE_HIDEABLE = 4,
    TABLE_SORTABLE = 8,
    TABLE_NOSAVEDSETTINGS = 16,
    TABLE_CONTEXTMENUINBODY = 32,
    TABLE_ROWBG = 64,
    TABLE_BORDERSINNERH = 128,
    TABLE_BORDERSOUTERH = 256,
    TABLE_BORDERSINNERV = 512,
    TABLE_BORDERSOUTERV = 1024,
    TABLE_BORDERSH = 2048,
    TABLE_BORDERSV = 4096,
    TABLE_BORDERSINNER = 8192,
    TABLE_BORDERSOUTER = 16384,
    TABLE_BORDERS = 32768,
    TABLE_NOBORDERSINBODY = 65536,
    TABLE_NOBORDERSINBODYUNTILRESIZE = 131072,
    TABLE_SIZINGFIXEDFIT = 262144,
    TABLE_SIZINGFIXEDSAME = 524288,
    TABLE_SIZINGSTRETCHPROP = 1048576,
    TABLE_SIZINGSTRETCHSAME = 2097152,
    TABLE_NOHOSTEXTENDX = 4194304,
    TABLE_NOHOSTEXTENDY = 8388608,
    TABLE_NOKEEPCOLUMNSVISIBLE = 16777216,
    TABLE_PRECISEWIDTHS = 33554432,
    TABLE_NOCLIP = 67108864,
    TABLE_PADOUTERX = 134217728,
    TABLE_NOPADOUTERX = 268435456,
    TABLE_NOPADINNERX = 536870912,
    TABLE_SCROLLX = 1073741824,
    TABLE_SCROLLY = 2147483648,
    TABLE_SORTMULTI = 4294967296,
    TABLE_SORTTRISTATE = 8589934592
}

---@enum InputFlags
imgui = {
    INPUTFLAGS_NONE = 0,
    INPUTFLAGS_CHARSDECIMAL = 1,
    INPUTFLAGS_CHARSHEXADECIMAL = 2,
    INPUTFLAGS_CHARSUPPERCASE = 4,
    INPUTFLAGS_CHARSNOBLANK = 8,
    INPUTFLAGS_AUTOSELECTALL = 16,
    INPUTFLAGS_ENTERRETURNSTRUE = 32,
    INPUTFLAGS_CALLBACKCOMPLETION = 64,
    INPUTFLAGS_CALLBACKHISTORY = 128,
    INPUTFLAGS_CALLBACKALWAYS = 256,
    INPUTFLAGS_CALLBACKCHARFILTER = 512,
    INPUTFLAGS_ALLOWTABINPUT = 1024,
    INPUTFLAGS_CTRLENTERFORNEWLINE = 2048,
    INPUTFLAGS_NOHORIZONTALSCROLL = 4096,
    INPUTFLAGS_ALWAYSOVERWRITE = 8192,
    INPUTFLAGS_READONLY = 16384,
    INPUTFLAGS_PASSWORD = 32768,
    INPUTFLAGS_NOUNDOREDO = 65536,
    INPUTFLAGS_CHARSSCIENTIFIC = 131072,
    INPUTFLAGS_CALLBACKRESIZE = 262144,
    INPUTFLAGS_CALLBACKEDIT = 524288,
    INPUTFLAGS_ESCAPECLEARSALL = 1048576
}

---@enum CondFlags
imgui = {
    COND_NONE = 0,
    COND_ALWAYS = 1,
    COND_ONCE = 2,
    COND_FIRSTUSEEVER = 4,
    COND_APPEARING = 8
}

---@enum DirFlags
imgui = {
    DIR_NONE = 0,
    DIR_LEFT = 1,
    DIR_RIGHT = 2,
    DIR_UP = 4,
    DIR_DOWN = 8
}

---@enum Key
imgui = {
    KEY_TAB = 0,
    KEY_LEFTARROW = 1,
    KEY_RIGHTARROW = 2,
    KEY_UPARROW = 3,
    KEY_DOWNARROW = 4,
    KEY_PAGEUP = 5,
    KEY_PAGEDOWN = 6,
    KEY_HOME = 7,
    KEY_END = 8,
    KEY_INSERT = 9,
    KEY_DELETE = 10,
    KEY_BACKSPACE = 11,
    KEY_SPACE = 12,
    KEY_ENTER = 13,
    KEY_ESCAPE = 14,
    KEY_KEYPADENTER = 15,
    KEY_A = 16,
    KEY_C = 17,
    KEY_V = 18,
    KEY_X = 19,
    KEY_Y = 20,
    KEY_Z = 21
}

---@enum GlyphRanges
imgui = {
    GLYPH_RANGES_DEFAULT = 0,
    GLYPH_RANGES_GREEK = 1,
    GLYPH_RANGES_KOREAN = 2,
    GLYPH_RANGES_JAPANESE = 3,
    GLYPH_RANGES_CHINESEFULL = 4,
    GLYPH_RANGES_CHINESESIMPLIFIEDCOMMON = 5,
    GLYPH_RANGES_CYRILLIC = 6,
    GLYPH_RANGES_THAI = 7,
    GLYPH_RANGES_VIETNAMESE = 8
}