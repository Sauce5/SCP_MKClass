#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

#include "scp_gui.h"
#include "imgui.h"

using namespace std;
namespace fs = std::filesystem;

namespace SCPGUI {
    static bool classified = false;

    static char mkc_command[256];
    static string mkc_out = "";
    static string mkc_log = "";

    static string sel_lib;
    static string sel_spec;

    // just some shorthand for seperating widgets
    void seperator() {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    /**
     * Run mkclass with arguments
     * 
     * @param sel_out = path to MKClass's output file
     * @param sel_log = path to MKClass's log file
    */
    void run_command(string sel_out, string sel_log) {
        static int err;
        err = system(mkc_command);
        ofstream ofile;
        if (err) {
            mkc_out = "";
            mkc_log = "INPUT ERROR";
            classified = false;
        } else {
            ifstream ifile;

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
            classified = true;
        }

        // clear mkc.log and mkc.out
        ofile.open(sel_out);
        ofile << "";
        ofile.close();
        ofile.open(sel_log);
        ofile << "";
        ofile.close();
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
        std::vector<string> paths = {};
        string path = "../local/mkclass/" + spectra_libs[lib_selected];
        for (const auto & entry : fs::directory_iterator(path)) {
            string path_str = entry.path().string();
            path_str = path_str.erase(0, path.length()+1);
            if (path_str.length() >= 4 && path_str.substr(path_str.length() - 4) == ".rbn")
                paths.push_back(path_str);
        }

        std::vector<string> spectra = paths;
        int spectra_n = spectra.size(); //5;
        static int spectrum_selected = 0;
        ImGui::Text("Spectrum:");
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
        ImGui::InputInt(" ", &iter, 1, 1);
        
        seperator();

        // submit button
        if (ImGui::Button("Classify")) {
            // build the command
            sel_lib = spectra_libs[lib_selected];
            sel_spec = spectra[spectrum_selected];
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

    void renderVisWindow() {
        // open spectrum file, load it into string variable
        ifstream ifile;
        ifile.open("../local/mkclass/" + sel_lib + "/" + sel_spec);
        string str;
        vector<string> out = vector<string>();
        while (getline(ifile, str)) {
            out.push_back(str + "\n");
        }
        ifile.close();

        // parse spectrum string into arrays of wavelength (x) and flux (y) values
        float *x0, *y0;
        x0 = new float[out.size()];
        y0 = new float[out.size()];
        for (int i = 0; i < (int) out.size(); i++) {
            string currX = "";
            string currY = "";
            int j;
            for (j = 0; out[i][j] != ' '; j++) {
                currX += out[i][j];
            }

            for (j++; j < (int) out[i].length(); j++) {
                currY += out[i][j];
            }

            x0[i] = atof(currX.c_str());
            y0[i] = atof(currY.c_str());
        }

        // plug spectrum data into line graph
        ImGui::PlotLines("Flux", y0, out.size(), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(975.0f, -50.0f));
        string start = classified ? "Wavelength (starts at " + to_string(x0[0]) + ")" : "Wavelength";
        ImGui::Text(start.c_str());
    }

    void renderConsoleLog() {
        ImGui::Text(mkc_out.c_str());

        seperator();
        
        ImGui::Text(mkc_log.c_str());
    }
}
