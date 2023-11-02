#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "scp_gui.h"
#include "imgui.h"

using namespace std;

namespace SCPGUI {
    static char mkc_command[256];
    static string mkc_out = "";
    static string mkc_log = "";

    // just some shorthand for seperating widgets
    void seperator() {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    void run_command(string sel_out, string sel_log) {
        static int err;
        err = system(mkc_command); // this does the actual execution - probably not the best way to do this though
        if (err) {
            mkc_out = "";
            mkc_log = "INPUT ERROR";
        } else {
            ifstream ifile;
            ofstream ofile;

            // open ./mkc.out file, send ./mkc.out to variable out
            ifile.open(sel_out);
            string str, out = "";
            while (getline(ifile, str)) {
                out += str + "\n";
            }
            ifile.close();

            // send ./mkc.log to log
            ifile.open(sel_log);
            str = "";
            string log = "";
            while (getline(ifile, str)) {
                log += str + "\n";
            }
            ifile.close();

            mkc_out = out;
            mkc_log = log;

            // clear mkc.log and mkc.out
            ofile.open(sel_out);
            ofile << "";
            ofile.close();
            ofile.open(sel_log);
            ofile << "";
            ofile.close();
        }
    }

    void renderSidebar() {
        // select library
        static string spectra_libs[] = {
            "libr18",
            "libnor36",
            "libr18_27",
            "libr18_30",
            "libr18_225",
            "libr36"
        };
        static int spectra_libs_n = 6;
        static int lib_selected = 0;
        ImGui::Text("Library (default: libr18)");
        if (ImGui::BeginListBox("1")) {
            for (int n = 0; n < spectra_libs_n; n++) {
                const bool is_selected = (lib_selected == n);
                if (ImGui::Selectable(spectra_libs[n].c_str(), is_selected))
                    lib_selected = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        
        seperator();

        // select spectrum
        string spectra[] = {
            "t010l30p00.rbn",
            "tXXXlYYp00-1.rbn",
            "tXXXlYYp00-2.rbn",
            "tXXXlYYp00-3.rbn",
            "tXXXlYYp00-4.rbn"
        };
        int spectra_n = 5;
        static int spectrum_selected = 0;
        ImGui::Text("Spectrum (default: t010l30p00.rbn)");
        if (ImGui::BeginListBox("2")) {
            for (int n = 0; n < spectra_n; n++) {
                const bool is_selected = (spectrum_selected == n);
                if (ImGui::Selectable(spectra[n].c_str(), is_selected))
                    spectrum_selected = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }ImGui::EndListBox();
        }

        seperator();

        // output file, log file
        ImGui::Text("Output: mkc.out | Log: mkc.log");
        
        seperator();

        // roughtype
        ImGui::Text("Roughtype: (default: 2)");
        static int roughtype = 2;
        ImGui::RadioButton("1", &roughtype, 1);
        ImGui::RadioButton("2", &roughtype, 2);
        
        seperator();

        // number of iterations
        ImGui::Text("Number of Iterations");
        static int iter = 3;
        ImGui::InputInt(" ", &iter, 1, 1); // doens't limit user input to positive numbers yet
        
        seperator();

        // submit button
        if (ImGui::Button("Classify")) {
            // build the command
            string sel_lib = spectra_libs[lib_selected];
            string sel_spec = spectra[spectrum_selected]; //"t010l30p00.rbn"; // default spectrum for now
            string sel_out = "./mkc.out";
            string sel_log = "./mkc.log";
            int sel_roughtype = roughtype;
            int sel_iter = iter;
            sprintf(mkc_command, "mkclass ../local/mkclass/%s/%s %s %s %s %d %d",
                sel_lib.c_str(),
                sel_spec.c_str(),
                sel_lib.c_str(),
                sel_out.c_str(),
                sel_log.c_str(),
                sel_roughtype,
                sel_iter);
            
            run_command(sel_out, sel_log);
        }
    }

    void renderMainWindow() {
        ImGui::Text(mkc_out.c_str());

        seperator();
        
        ImGui::Text(mkc_log.c_str());
    }
}
