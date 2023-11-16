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
        std::vector<string> paths = {};
        string path = "../local/mkclass/" + spectra_libs[lib_selected];
        for (const auto & entry : fs::directory_iterator(path)) {
            string path_str = entry.path().string();
            path_str = path_str.erase(0, path.length()+1);
            paths.push_back(path_str);
        }

        /*string spectra[] = {
            "t010l30p00.rbn",
            "tXXXlYYp00-1.rbn",
            "tXXXlYYp00-2.rbn",
            "tXXXlYYp00-3.rbn",
            "tXXXlYYp00-4.rbn"
        };*/
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
        ImGui::InputInt(" ", &iter, 1, 1); // doens't limit user input to positive numbers yet
        
        seperator();

        // submit button
        if (ImGui::Button("Classify")) {
            // build the command
            sel_lib = spectra_libs[lib_selected];
            sel_spec = spectra[spectrum_selected]; //"t010l30p00.rbn"; // default spectrum for now
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
        ifstream ifile;
        ifile.open("../local/mkclass/" + sel_lib + "/" + sel_spec);
        string str;
        vector<string> out = vector<string>();
        while (getline(ifile, str)) {
            out.push_back(str + "\n");
        }
        ifile.close();

        float *x0, *y0;
        x0 = new float[out.size()];
        y0 = new float[out.size()];

        for (int i = 0; i < out.size(); i++) {
            string currX = "";
            string currY = "";
            int j;
            for (j = 0; out[i][j] != ' '; j++) {
                currX += out[i][j];
            }

            for (j++; j < out[i].length(); j++) {
                currY += out[i][j];
            }

            x0[i] = atof(currX.c_str());
            y0[i] = atof(currY.c_str());
        }

        /*
        long l;
        char buffer[200],*tmp;
        FILE *in;

        in = fopen(infile,"r");
        l = 0;
        while(fgets(buffer,190,in) != NULL) {
        if(buffer[0] == '#') continue;
        tmp = strtok(buffer," ");
        x[l] = atof(tmp);
        tmp = strtok(NULL," ");
        y[l] = atof(tmp);
        l++;
        }
        fclose(in);
        */

        ImGui::PlotLines("Spectrum Vis Test", y0, out.size(), 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0.0f, -5.0f));
    }

    void renderConsoleLog() {
        ImGui::Text(mkc_out.c_str());

        seperator();
        
        ImGui::Text(mkc_log.c_str());
    }
}
