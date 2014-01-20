#include "ScriptPackage.h"
#include <iostream>
#include <string>
#include "FileSystem.h"
#include "loadscript.h"
#include <angelscript.h>

#include "Convert/convertstring.h"
#include "Convert/units.h"
#include "DefaultPlugin.h"
#include "ScriptWPILib.h"
#include "ftp/FTPClient.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#endif

std::string lastwriteconfig;
std::string execdir;
std::string teamnumber;

typedef bool(*command)(std::vector<std::string>, std::string);

void addCommands();
void addCommand(std::string aliases, command c);

std::string lastwrotepath;

std::vector<std::pair<std::string, command> > commands;
bool quit = false;

ScriptEngine* g_engine;
asIScriptContext* ctx;
ScriptPackage* g_package;

std::string ipFromTeamNumber(std::string tnum)
{
    std::string ip = tnum;
    ip.insert(2, 1, '.');
    std::string ip1 = ip.substr(0, 3);
    ip = ip.substr(3);
    if(ip.substr(0, 1) == "0")
    {
        ip = ip.substr(1);

    }

    ip = ip1 + ip;

    ip = "10."+ip+".2";
    return ip;

}

std::vector<std::string> parseParams(std::string line)
{
    std::vector<std::string> params;
    while(line.find(" ") < line.size())
    {
        std::string param = line.substr(0, line.find(" "));
        if(param != "")
        {
            params.push_back(param);
        }
        line = line.substr(line.find(" ") + 1);

    }

    params.push_back(line);

    std::vector<std::string>::iterator it;
    for(it = params.begin(); it != params.end(); it++)
    {
        if((*it) == "" || (*it) == " ")
        {
            params.erase(it);
            it--;

        }

    }

    return params;

}

std::string getNameFromPath(std::string path)
{
    std::string name;
    name = path.substr(path.rfind("/")+1);
    return name;

}

bool quitCMD(std::vector<std::string> params, std::string line)
{
    quit = true;
    return true;

}

bool write(std::vector<std::string> params, std::string line)
{
    if(!(params.size() >= 2))
    {
        std::cout << "usage: <directory> <output> ...\nyou can add mode -e files ... to exclude and -a files ... to add\n";
        return false;

    }

    int dirmode = 1; //0 = fromexec 1 = absolute use -f for fromexec
    std::string dir = params[1];
    std::string output = "FRC_UserProgram.scpkg";
    //0 = outputs, 1 = excludes, 2 = add
    int mode = 0;

    if(params.size() > 2)
    {
        std::string out = params[2];
        out = out.substr(0, 1);
        if(out != "-")
        {
            output = params[2];

        }

    }

    std::vector<std::string> excludes;
    std::vector<std::string> includes;

    for(unsigned int i = 2; i < params.size(); i++)
    {
        if(params[i] == "-e")
        {
            mode = 1;

        }
        else if(params[i] == "-a")
        {
            mode = 2;

        }
        else if(params[i] == "-f")
        {
            dirmode = 0;

        }
        else if(mode == 1)
        {
            excludes.push_back(params[i]);

        }
        else if(mode == 2)
        {
            includes.push_back(params[i]);

        }

    }

    std::vector<std::string> files;
    getFilesFromPath(dirmode == 0 ? execdir + "/" : "" + dir, files);

    PackageArchive archive;
    for(unsigned int i = 0; i < files.size(); i++)
    {
        if(ScriptPackage::isConfig(files[i]) || ScriptPackage::isScript(files[i]))
        {
            bool next = false;
            for(unsigned int u = 0; u < excludes.size(); u++)
            {
                if(getNameFromPath(files[i]) == getNameFromPath(excludes[u]))
                {
                    next = true;
                    break;

                }

            }

            if(next)
            {
                continue;

            }

            std::cout << "adding file: " << files[i] << "\n";
            archive.add(getNameFromPath(files[i]), loadScript(files[i]));

        }

    }

    for(unsigned int i = 0; i < includes.size(); i++)
    {
        std::string f = loadScript(includes[i]);
        if(f != "")
        {
            archive.add(getNameFromPath(includes[i]), f);

        }

    }

    if(archive.write(dirmode == 0 ? execdir + "/" : "" + dir + "/" + output) != PackageArchive::NoError)
    {
        std::cout << "failed to write package.\n";
        return false;

    }
    std::string dirmodestr = dirmode == 0 ? " -f" : "";
    lastwriteconfig = dir + " " + output + dirmodestr;
    lastwriteconfig += " -a";
    for(unsigned int i = 0; i < includes.size(); i++)
    {
        lastwriteconfig += " " + includes[i];

    }

    lastwriteconfig += " -e";
    for(unsigned int i = 0; i < excludes.size(); i++)
    {
        lastwriteconfig += " " + excludes[i];

    }

    lastwrotepath = dir+"/"+output;
    std::cout << "package outputed at: " << dir + "/" + output << "\n";
    return true;

}

bool load(std::vector<std::string> params, std::string line)
{
    if(params.size() != 2 && lastwrotepath == "")
    {
        std::cout << "usage: load <packagepath>\nif you don't specify the path it will use the last loaded/wrote path\n";
        return false;

    }

    std::string path = lastwrotepath;
    if(params.size() == 2)
    {
        path = params[1];

    }

    ScriptPackage* temp = new ScriptPackage;
    ScriptPackage::Error error;
    error = temp->load(path);
    if(error != ScriptPackage::NotBuilt)
    {
        std::cout << "package failed to load\n";
        delete temp;
        return false;

    }

    ctx->Release();
    if(g_package != NULL)
    {
        g_package->unload();

    }
    ctx = g_engine->get()->CreateContext();

    error = temp->build(g_engine);
    if(error != ScriptPackage::NoError)
    {
        std::cout << "failed to build package error: " << error << "\n";
        temp->unload();
        if(g_package != NULL)
        {
            g_package->build(g_engine);

        }

        delete temp;
        return false;

    }

    if(g_package != NULL)
    {
        delete g_package;
    }

    g_package = temp;
    lastwrotepath = g_package->getPath();
    std::cout << "new package loaded use list command to see available routines\n";
    return true;

}

bool loadRobot(std::vector<std::string>, std::string);//random prototype!

bool deploy(std::vector<std::string> params, std::string line)
{
    if(g_package == NULL || !g_package->isValid())
    {
        std::cout << "a package needs to be loaded to deploy\n";
        return false;

    }

    if((params.size() != 1 && teamnumber != "") && params.size() != 2 && params.size() != 3)
    {
        std::cout << "usage: deploy <teamnumber> <destfile>\n";
        return false;

    }

    std::string ip = teamnumber;
    if(params.size() != 1)
    {
        ip = params[1];

    }

    std::string org = ip;

    ip = ipFromTeamNumber(ip);

    std::string destfile = "/FRC_UserProgram.scpkg";

    if(params.size() == 3)
    {
        destfile = "/" + params[2];

    }

    nsFTP::CFTPClient ftpClient;
    nsFTP::CLogonInfo logonInfo(ip);

    if(!ftpClient.Login(logonInfo))
    {
        std::cout << "failed to logon to: " << ip << "\n";
        return false;

    }

    ftpClient.Delete("/backup.scpkg");
    ftpClient.Rename("/FRC_UserProgram.scpkg", "/backup.scpkg");
    if(!ftpClient.UploadFile(g_package->getPath(), destfile))
    {
        std::cout << "failed to upload file\n";
        return false;

    }

    ftpClient.Logout();

    std::cout << "deployed " << destfile << "\n";

    teamnumber = org;

    params.clear();
    params.push_back("loadrobot");
    params.push_back(destfile);
    if(!loadRobot(params, "reloadrobot " + destfile))
    {
        return false;

    }

    return true;

}

bool run(std::vector<std::string> params, std::string line)
{
    if(g_package == NULL || !g_package->isValid())
    {
        std::cout << "a package needs to be loaded to run a routine\n";
        return false;

    }

    if(!(params.size() >= 2))
    {
        std::cout << "run <routinename> ...\n";
        return false;

    }

    ScriptRoutine* routine = g_package->getRoutine(params[1]);
    if(routine == NULL)
    {
        std::cout << "no routine named: " << params[1] << "\n";
        return false;

    }

    for(unsigned int i = 0; i < routine->getNumOfHooks(); i++)
    {
        asIScriptFunction* func = routine->getHook(i);
        if(func != NULL)
        {
            ScriptRobot robot;
            if(ctx->Prepare(func) < 0)
            {
                std::cout << "failed to prepare a function\n";
                ctx->Unprepare();
                return false;

            }

            ctx->SetArgAddress(0, &robot);
            ctx->Execute();

        }

    }

    return true;

}

bool list(std::vector<std::string> params, std::string line)
{
    if(g_package == NULL || !g_package->isValid())
    {
        std::cout << "a package needs to be loaded to list all available routines\n";
        return false;

    }

    std::list<ScriptRoutine> routines = g_package->getRoutines();

    std::list<ScriptRoutine>::iterator it;
    std::cout << "\n\nroutines:\n";
    for(it = routines.begin(); it != routines.end(); it++)
    {
        std::cout << "\t" << it->getName() << "\n";

    }

    return true;

}

bool help(std::vector<std::string> params, std::string line)
{
    std::cout << "\n\ncommands:\n";
    for(unsigned int i = 0; i < commands.size(); i++)
    {
        std::cout << "\t" << commands[i].first << "\n";

    }

    return false;

}

bool config(std::vector<std::string> params, std::string line)
{
    if((params.size() != 2 && lastwriteconfig != "") && !(params.size() > 2))
    {
        std::cout << "usage: config <makeconfig> ...\n";
        return false;

    }

    std::string output = params[1];

    std::ofstream file;
    file.open(output.c_str());
    if(!file)
    {
        std::cout << "couldn't output to: " << output << "\n";
        return false;

    }

    if(params.size() == 2 && lastwriteconfig != "")
    {
        file << lastwriteconfig;

    }
    else
    {
        for(unsigned int i = 2; i < params.size(); i++)
        {
            file << params[i] << " ";

        }

    }
    file.close();
    return true;

}

bool make(std::vector<std::string> params, std::string line)
{
    if(params.size() != 2)
    {
        std::cout << "usage: make <makeconfig>\n";
        return false;

    }

    std::fstream file;
    file.open(params[1].c_str());
    if(!file)
    {
        std::cout << "couldn't open make config: " << params[1] << "\n";
        return false;

    }

    std::string l;

    while(!file.eof())
    {
        std::string t;
        file >> t;
        l += " " + t;

    }

    l = "write " + l;

    std::vector<std::string> p;
    std::string fullline = l;
    p = parseParams(l);

    return write(p, fullline);

}

bool setTeamNumber(std::vector<std::string> params, std::string line)
{
    if(params.size() != 2)
    {
        std::cout << "usage: teamnumber <teamnumber>\n";
        return false;

    }

    teamnumber = params[1];
    std::cout << teamnumber << " generates this ip: " << ipFromTeamNumber(teamnumber) << "\n";
    return true;

}

bool reloadRobot(std::vector<std::string> params, std::string line)
{
    if((params.size() != 1 && teamnumber != "") && params.size() != 2)
    {
        std::cout << "usage: reloadRobot <teamnumber>\n";
        return false;

    }

    std::string ip = teamnumber;
    if(params.size() != 1)
    {
        ip = params[1];

    }

    std::string org = ip;
    ip = ipFromTeamNumber(ip);
    std::string command = "reloadSCPKG";

    //TODO: opensocket, sendcommand

    //TODO:
    //if success do this:
    //        teamnumber = org; and return true;
    return false;

}

bool loadRobot(std::vector<std::string> params, std::string line)
{
    if((params.size() != 2 && teamnumber != "") && params.size() != 3)
    {
        std::cout << "usage: loadRobot <scriptpackage> <teamnumber>\n";
        return false;

    }

    std::string ip = teamnumber;
    if(params.size() != 2)
    {
        ip = params[2];

    }

    std::string org = ip;
    ip = ipFromTeamNumber(ip);
    std::string command = "loadSCPKG";

    //TODO: opensocket, sendcommand

    //TODO:
    //if success do this:
    //        teamnumber = org; and return true;
    return false;

}

bool makedeploy(std::vector<std::string> params, std::string line)
{
    if((params.size() != 2 && teamnumber != "")  && params.size() != 3 && params.size() != 4)
    {
        std::cout << "usage: makedeploy <makeconfig> <teamnumber> <destfile>\n";
        return false;

    }

    std::string makeconfig = params[1];
    std::string tnum = teamnumber;
    if(params.size() != 2)
    {
        tnum = params[2];

    }

    std::string destfile = "/FRC_UserProgram.scpkg";
    if(params.size() == 4)
    {
        destfile = "/" + params[3];

    }

    std::vector<std::string> p;
    p.push_back("make");
    p.push_back(makeconfig);
    if(!make(p, "make " + makeconfig))
    {
        return false;

    }

    p.clear();
    p.push_back("load");
    if(!load(p, "load"))
    {
        return false;

    }

    p.clear();
    p.push_back("deploy");
    p.push_back(teamnumber);
    p.push_back(destfile);
    return deploy(p, "deploy " +tnum + " "+destfile);

}

int main(int argc, char *argv[])
{
#ifdef _WIN32
	  char c[MAX_PATH];
	  GetModuleFileName(NULL, c, MAX_PATH);
	  execdir = std::string(c);
#elif __linux__
	char* c  = get_current_dir_name();
	execdir = std::string(c);
	free(c);

#endif
    teamnumber = argc >= 2 ? argv[1] : "";

    execdir = execdir.substr(0, execdir.rfind("\\"));
    std::cout << "team number: " << teamnumber << "\n";
    std::cout << "working dir: " << execdir << "\n";
    g_engine = new ScriptEngine;
    g_engine->addPlugin(new DefaultPlugin);
    g_engine->addPlugin(new ScriptWPILib);

    ctx = g_engine->get()->CreateContext();

    g_package = NULL;

    addCommands();
    while(!quit)
    {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if(line == "")
        {
            continue;

        }

        std::vector<std::string> params;
        std::string fullline = line;
        params = parseParams(line);

        for(unsigned int i = 0; i < commands.size(); i++)
        {
            if(commands[i].first.find(params[0]+";") < commands[i].first.size())
            {
                commands[i].second(params, fullline);

            }

        }

    }

    if(g_package != NULL)
    {
        delete g_package;

    }

    ctx->Release();
    g_engine->remRef();

}

void addCommands()
{
    addCommand("quit;exit;kill;q", quitCMD);
    addCommand("write;package;w;", write);
    addCommand("load;compile;read;l;", load);
    addCommand("run;r;", run);
    addCommand("deploy;put;d;ftp;", deploy);
    addCommand("list;routines;", list);
    addCommand("help;h;", help);
    addCommand("create;config;c;", config);
    addCommand("make;m;", make);
    addCommand("makedeploy;md;mdeploy;", makedeploy);
    addCommand("reloadrobot;reload;rr;", reloadRobot);
    addCommand("loadrobot;loadr;lr;", loadRobot);
    addCommand("teamnumber;number;team;t;n;tn;", setTeamNumber);

}

void addCommand(std::string aliases, command c)
{
    commands.push_back(std::pair<std::string, command>(aliases, c));

}
