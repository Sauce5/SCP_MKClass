#include <iostream>
#include <fstream>
#include <string>

#include "scp_gui.h"
#include "imgui.h"

using namespace std;

namespace SCPGUI {
    void renderUI() {
        /*
        // for now, will be just like command-line (w/o exe command)
        ImGui::Text("File name (tXXXlXXpXX.rbn), but only the ones from libr18 for now: ");
        static char com[64] = "";
        ImGui::InputText(" ", com, 64);

        // create command string, store in buffer
        static char buffer[256];
        // this command is hardcoded for now
        sprintf(buffer, "mkclass ../local/mkclass/libr18/%s libr18 ../test.out ../test.log 2 3", com);

        // button to submit com[] to ../mkclassv107/mkclass.exe
        static string out = "";
        if (ImGui::Button("button 1")) {
            // call command (stored in buffer)
            static int err;
            err = system(buffer); // this does the actual execution - probably not the best way to do this though
            if (err) {
                out = "oh no!";
            } else {
                // open test.out file, send test.out to variable out
                static ifstream file;
                file.open("../test.out");
                string all = "", str;
                while (getline(file, str)) {
                    all += str + "\n";
                }

                // do the same thing, but with the test.log file

                out = all;
                file.close();
            }
        }

        // will be output for MKClass
        ImGui::Text(out.c_str());
        */
    }
}
