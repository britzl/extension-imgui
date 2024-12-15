# API reference

* src
  * [extension_imgui](#extension_imgui)

# src

## extension_imgui
*File: `imgui/src/extension_imgui.cpp`*




### image_b64_decode(data,length)
ImageB64Decode 


PARAMS
* `data` [`string`] - 
* `length` [`number`] - 

RETURNS
* `decoded` [`string`] - Data


### image_load_data(filename,data,data_length)
ImageLoadData 


PARAMS
* `filename` [`string`] - 
* `data` [`string`] - 
* `data_length` [`number`] - 

RETURNS
* `texture_id` [`number`] - 
* `width` [`number`] - 
* `height` [`number`] - 


### image_load(filename)
ImageLoad 


PARAMS
* `filename` [`string`] - 

RETURNS
* `texture_id` [`number`] - 
* `width` [`number`] - 
* `height` [`number`] - 


### add_line(x1,y1,x2,y2,r,g,b,a)
DrawListAddLine 


PARAMS
* `x1` [`number`] - 
* `y1` [`number`] - 
* `x2` [`number`] - 
* `y2` [`number`] - 
* `r` [`number`] - 
* `g` [`number`] - 
* `b` [`number`] - 
* `a` [`number`] - 


### add_rect(x1,y1,x2,y2,r,g,b,a,thickness)
DrawListAddRect 


PARAMS
* `x1` [`number`] - 
* `y1` [`number`] - 
* `x2` [`number`] - 
* `y2` [`number`] - 
* `r` [`number`] - 
* `g` [`number`] - 
* `b` [`number`] - 
* `a` [`number`] - 
* `thickness` [`number`] - 


### set_display_size(w,h)
SetDisplaySize 


PARAMS
* `w` [`number`] - 
* `h` [`number`] - 


### set_mouse_input(x,y,down1,down2,down3,wheel)
SetMouseInput 


PARAMS
* `x` [`number`] - 
* `y` [`number`] - 
* `down1` [`number`] - 
* `down2` [`number`] - 
* `down3` [`number`] - 
* `wheel` [`number`] - 


### set_mouse_pos(x,y)
SetMousePos 


PARAMS
* `x` [`number`] - 
* `y` [`number`] - 


### set_mouse_button(index,down)
SetMouseButton 


PARAMS
* `index` [`number`] - 
* `down` [`number`] - 


### set_mouse_wheel(wheel)
SetMouseWheel 


PARAMS
* `wheel` [`number`] - 


### set_key_down(key,down)
SetKeyDown 


PARAMS
* `key` [`number`] - 
* `down` [`boolean`] - 


### set_key_modifier_ctrl(down)
SetKeyModifierCtrl 


PARAMS
* `down` [`boolean`] - 


### set_key_modifier_shift(down)
SetKeyModifierShift 


PARAMS
* `down` [`boolean`] - 


### set_key_modifier_alt(down)
SetKeyModifierAlt 


PARAMS
* `down` [`boolean`] - 


### set_key_modifier_super(down)
SetKeyModifierSuper 


PARAMS
* `down` [`boolean`] - 


### add_input_character(character)
AddInputCharacter 


PARAMS
* `character` [`string`] - 


### add_input_characters(characters)
AddInputCharacters 


PARAMS
* `characters` [`string`] - 


### tree_node(text,flags)
TreeNode 


PARAMS
* `text` [`string`] - 
* `flags` [`number`] - 


### tree_pop()
TreePop 



### push_id(text)
PushId 


PARAMS
* `text` [`string`] - 


### pop_id()
PopId 



### begin(title,is_open,flags)
Begin 


PARAMS
* `title` [`string`] - 
* `is_open` [`boolean`] - 
* `flags` [`number`] - 

RETURNS
* `result` [`bool`] - 
* `is_open` [`bool`] - 


### end()
End 



### set_next_window_size(width,height,cond)
SetNextWindowSize 


PARAMS
* `width` [`number`] - 
* `height` [`number`] - 
* `cond` [`number`] - 


### set_next_window_pos(x,y,cond)
SetNextWindowPos 


PARAMS
* `x` [`number`] - 
* `y` [`number`] - 
* `cond` [`number`] - 


### get_window_size()
GetWindowSize 


RETURNS
* `width` [`number`] - 
* `height` [`number`] - 


### get_window_pos()
GetWindowPos 


RETURNS
* `x` [`number`] - 
* `y` [`number`] - 


### is_window_focused(flags)
IsWindowFocused 


PARAMS
* `flags` [`number`] - 

RETURNS
* `focused` [`boolean`] - 


### get_content_region_avail()
GetContentRegionAvail 


RETURNS
* `x` [`number`] - 
* `y` [`number`] - 


### get_window_content_region_max()
GetWindowContentRegionMax 


RETURNS
* `x` [`number`] - 
* `y` [`number`] - 


### begin_child(title,width,border,flags)
BeginChild 


PARAMS
* `title` [`string`] - 
* `width` [`number`] - 
* `border` [`number`] - 
* `flags` [`number`] - 

RETURNS
* `result` [`bool`] - 


### end_child()
EndChild 



### begin_popup_context_item(id)
BeginPopupContextItem 


PARAMS
* `id` [`string`] - 

RETURNS
* `result` [`boolean`] - 


### begin_popup(id,flags)
BeginPopup 


PARAMS
* `id` [`string`] - 
* `flags` [`number`] - 

RETURNS
* `result` [`boolean`] - 


### begin_popup_modal(id,flags)
BeginPopupModal 


PARAMS
* `id` [`string`] - 
* `flags` [`number`] - 

RETURNS
* `result` [`boolean`] - 


### open_popup(id,flags)
OpenPopup 


PARAMS
* `id` [`string`] - 
* `flags` [`number`] - 

RETURNS
* `result` [`boolean`] - 


### close_current_popup()
CloseCurrentPopup 



### end_popup()
EndPopup 



### begin_drag_drop_source(flags)
BeginDragDropSource 


PARAMS
* `flags` [`number`] - 


### end_drag_drop_source()
EndDragDropSource 



### begin_drag_drop_target()
BeginDragDropTarget 



### end_drag_drop_target()
EndDragDropTarget 



### set_drag_drop_payload(type,payload)
SetDragDropPayload 


PARAMS
* `type` [`string`] - 
* `payload` [`string`] - 


### accept_drag_drop_payload(type,flags)
AcceptDragDropPayload 


PARAMS
* `type` [`string`] - 
* `flags` [`number`] - 


### begin_combo(label,preview)
BeginCombo 


PARAMS
* `label` [`string`] - 
* `preview` [`string`] - 


### end_combo()
EndCombo 



### combo(label,current,items)
Combo 


PARAMS
* `label` [`string`] - 
* `current` [`number`] - 
* `items` [`table`] - 

RETURNS
* `result` [`boolean`] - 
* `r` [`numbe`] - 


### begin_table()
BeginTable 



### end_table()
EndTable 



### table_headers_row()
TableHeadersRow 



### table_setup_column()
TableSetupColumn 



### table_set_column_index()
TableSetColumnIndex 



### table_next_column()
TableNextColumn 



### table_next_row()
TableNextRow 



### table_setup_scroll_freeze()
TableSetupScrollFreeze 



### begin_tooltip()
BeginTooltip 



### end_tooltip()
EndTooltip 



### begin_tab_bar()
BeginTabBar 



### end_tab_bar()
EndTabBar 



### begin_tab_item()
BeginTabItem 



### end_tab_item()
EndTabItem 



### text()
Text 



### text_get_size()
TextGetSize 



### text_colored()
TextColored 



### input_text()
InputText 



### input_int()
InputInt 



### input_float()
InputFloat 



### input_double()
InputDouble 



### input_int4()
InputInt4 



### input_float3()
InputFloat3 



### input_float4()
InputFloat4 



### slider_float()
SliderFloat 



### selectable()
Selectable 



### button()
Button 



### button_image()
ButtonImage 



### checkbox()
Checkbox 



### radio_button()
RadioButton 



### begin_menu_bar()
BeginMenuBar 



### end_menu_bar()
EndMenuBar 



### begin_main_menu_bar()
BeginMainMenuBar 



### end_main_menu_bar()
EndMainMenuBar 



### begin_menu()
BeginMenu 



### end_menu()
EndMenu 



### menu_item()
MenuItem 



### same_line()
SameLine 



### new_line()
NewLine 



### bullet()
Bullet 



### indent()
Indent 



### unindent()
Unindent 



### spacing()
Spacing 



### separator()
Separator 



### get_cursor_screen_pos()
GetCursorScreenPos 



### plot_lines()
PlotLines 



### plot_histogram()
PlotHistogram 



### demo()
Demo 



### is_mouse_double_clicked()
IsMouseDoubleClicked 



### is_mouse_clicked()
IsMouseClicked 



### is_item_active()
IsItemActive 



### is_item_focused()
IsItemFocused 



### is_item_clicked()
IsItemClicked 



### is_item_double_clicked()
IsItemDoubleClicked 



### is_item_hovered()
IsItemHovered 



### get_item_rect_max()
GetItemRectMax 



### set_keyboard_focus_here()
SetKeyboardFocusHere 



### get_style()
GetStyle 



### set_style()
SetStyle 



### set_style_color()
SetStyleColor 



### push_style_color()
PushStyleColor 



### pop_style_color()
PopStyleColor 



### set_window_font_scale()
SetWindowFontScale 



### set_global_font_scale()
SetGlobalFontScale 



### scale_all_sizes()
ScaleAllSizes 



### set_cursor_pos()
SetCursorPos 



### push_item_width()
PushItemWidth 



### pop_item_width()
PopItemWidth 



### set_next_item_width()
SetNextItemWidth 



### calc_item_width()
CalcItemWidth 



### set_scroll_here_y()
SetScrollHereY 



### font_add_t_t_f_file()
FontAddTTFFile 



### font_add_t_t_f_data()
FontAddTTFData 



### font_push()
FontPush 



### font_pop()
FontPop 



### font_scale()
FontScale 



### get_frame_height()
GetFrameHeight 



### draw_line()
DrawLine 



### draw_rect()
DrawRect 



### draw_rect_filled()
DrawRectFilled 



### draw_progress_bar()
DrawProgressBar 



### set_rendering_enabled()
SetRenderingEnabled 



### want_capture_mouse()
WantCaptureMouse 



### want_capture_keyboard()
WantCaptureKeyboard 



### want_capture_text()
WantCaptureText 



### set_defaults()
SetDefaults 



### set_ini_filename()
SetIniFilename 



---

