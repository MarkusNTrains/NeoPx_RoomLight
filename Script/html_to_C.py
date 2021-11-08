import json

html_file_path = "../SD-Card/index.html"

# generate h file
h_file = open("../Website.h", "w+")
h_file.write("\n")
h_file.write("/*******************************************************************************\n")
h_file.write("Project   RoomLight\n")
h_file.write("\n")
h_file.write("  This is an OpenSource Project.\n")
h_file.write("  You can use, share or improve this project. If you improve this source code\n")
h_file.write("  please share with the comunity or at least with the author of the original\n")
h_file.write("  source code\n")
h_file.write("\n")
h_file.write("  Created 29. October 2021 by MarkusNTrains\n")
h_file.write("================================================================================\n")
h_file.write("$HeadURL:  $\n")
h_file.write("$Id:  $\n")
h_file.write("*******************************************************************************/\n")
h_file.write("\n")
h_file.write("\n")
h_file.write("#ifndef _WEBSITE_H\n")
h_file.write("#define _WEBSITE_H\n")
h_file.write("\n")
h_file.write("\n")
h_file.write("//-----------------------------------------------------------------------------\n")
h_file.write("// includes\n")
h_file.write("#include <Ethernet.h>\n")
h_file.write("\n")
h_file.write("\n")
h_file.write("//----------------------------------------------------------------------------\n")
h_file.write("// Function Prototypes\n")
h_file.write("void Website_SendToClient(EthernetClient* client);\n")
h_file.write("\n")
h_file.write("\n")
h_file.write("\n")
h_file.write("#endif  // _WEBSITE_H\n")


# generate cpp file
cpp_file = open("../Website.cpp", "w+")

cpp_file.write("/*******************************************************************************\n")
cpp_file.write("Project   RoomLight\n")
cpp_file.write("\n")
cpp_file.write("  This is an OpenSource Project.\n")
cpp_file.write("  You can use, share or improve this project. If you improve this source code\n")
cpp_file.write("  please share with the comunity or at least with the author of the original\n")
cpp_file.write("  source code\n")
cpp_file.write("\n")
cpp_file.write("  Created 29. October 2021 by MarkusNTrains\n")
cpp_file.write("================================================================================\n")
cpp_file.write("$HeadURL:  $\n")
cpp_file.write("$Id:  $\n")
cpp_file.write("*******************************************************************************/\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("//-----------------------------------------------------------------------------\n")
cpp_file.write("// includes\n")
cpp_file.write("#include \"Website.h\"\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("//-----------------------------------------------------------------------------\n")
cpp_file.write("// PROGMEM -> store data in flash\n")

nof_lines = 0
max_line_length = 0;
with open(html_file_path) as file:
    for line in file:
        line_str = ((line.rstrip()).lstrip(' \t'))
        line_str = json.dumps(line_str)
        # print(line_str)
        nof_lines += 1
        cpp_file.write("const char line" + str(nof_lines) + "[] PROGMEM = " + line_str + ";\n")

        if max_line_length < len(line_str):
            max_line_length = len(line_str)

# add footer
from datetime import date
footer_a = []
footer_a.append("<html>")
footer_a.append("<footer>")
footer_a.append("    <div>")
footer_a.append("        <br>")
footer_a.append("        <br>")
footer_a.append("        <hr>")
footer_a.append("        " + date.today().strftime("%d. %B %Y") + " &nbsp; &copy; MarkusNTrains")
footer_a.append("        <a href='mailto:markusntrains@gmx.ch'>E-Mail</a>")
footer_a.append("    </div>")
footer_a.append("</footer>")
footer_a.append("</html>")

for line in footer_a:
    line_str = ((line.rstrip()).lstrip(' \t'))
    line_str = json.dumps(line_str)
    # print(line_str)
    nof_lines += 1
    cpp_file.write("const char line" + str(nof_lines) + "[] PROGMEM = " + line_str + ";\n")

    if max_line_length < len(line_str):
        max_line_length = len(line_str)



cpp_file.write("\n")
cpp_file.write("const char* const website_a[] PROGMEM = {")
for line_number in range(nof_lines - 1):
    cpp_file.write("line" + str(line_number + 1) + ", ")

cpp_file.write("line" + str(nof_lines))
cpp_file.write("};\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("//-----------------------------------------------------------------------------\n")
cpp_file.write("// define\n")
cpp_file.write("#define WEBSITE_NOF_LINES " + str(nof_lines) + "\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("//-----------------------------------------------------------------------------\n")
cpp_file.write("// static module variable\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("\n")
cpp_file.write("//*****************************************************************************\n")
cpp_file.write("// description:\n")
cpp_file.write("//   Website_SendToClient\n")
cpp_file.write("//*****************************************************************************\n")
cpp_file.write("void Website_SendToClient(EthernetClient* client)\n")
cpp_file.write("{\n")
cpp_file.write("	uint32_t line = 0;\n")
cpp_file.write("    char line_str[" + str(max_line_length + 1) + "];\n")
cpp_file.write("\n")
cpp_file.write("	for (line = 0; line < WEBSITE_NOF_LINES; line++)\n")
cpp_file.write("	{\n")
cpp_file.write("        strcpy_P(line_str, (char*)pgm_read_word(&(website_a[line])));\n")
cpp_file.write("		client->println(line_str);\n")
cpp_file.write("	}\n")
cpp_file.write("}\n")
