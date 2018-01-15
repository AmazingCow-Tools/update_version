//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : update_version.cpp                                            //
//  Project   : update_version                                                //
//  Date      : Jan 15, 2018                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amaizingcow.com                              //
//  Copyright : AmazingCow - 2018                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

// std
#include <iostream>
#include <string>
#include <vector>
#include <utility>
// Amazing Cow Libs
#include "CoreFile/CoreFile.h"
#include "CoreFS/CoreFS.h"
#include "CoreString/CoreString.h"


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
// Error Output.
void show_error(const std::string &msg) noexcept
{
    std::cout << msg << std::endl;
    exit(1);
}


void show_usage() noexcept
{
    auto msg = "update_version <project-name> <version Major.Minor.Revision> <Header-Path> [Doxy-Path]";
    show_error(msg);
}


//------------------------------------------------------------------------------
// File management.
std::vector<std::string> read_file(const std::string &filepath) noexcept
{
    if(CoreFS::IsFile(filepath))
        return CoreFile::ReadAllLines(filepath);

    return std::vector<std::string>();
}


void write_file(
    const std::vector<std::string> &contents,
    const std::string              &filepath) noexcept
{
    if(!CoreFS::IsFile(filepath))
        return;

    CoreFile::WriteAllLines(filepath, contents);
}


//------------------------------------------------------------------------------
// Replace
std::pair<int, int> replace_header(
    const std::string        &projectName,
    std::vector<std::string> *pContents,
    int                       major,
    int                       minor,
    int                       revision) noexcept
{
    constexpr auto kMajor_Fmt    = "#define COW_%s_VERSION_MAJOR";
    constexpr auto kMinor_Fmt    = "#define COW_%s_VERSION_MINOR";
    constexpr auto kRevision_Fmt = "#define COW_%s_VERSION_REVISION";

    auto major_str    = CoreString::Format(kMajor_Fmt,    projectName);
    auto minor_str    = CoreString::Format(kMinor_Fmt,    projectName);
    auto revision_str = CoreString::Format(kRevision_Fmt, projectName);

    auto range = std::pair<int, int>();
    for(int i = 0; i < pContents->size(); ++i)
    {
        auto &line   = (*pContents)[i];
        auto changed = false;

        // Major.
        if(CoreString::Contains(line, major_str))
        {
            line    = CoreString::Format("%s    \"%d\"", major_str, major);
            changed = true;
        }
        // Minor.
        else if(CoreString::Contains(line, minor_str))
        {
            line    = CoreString::Format("%s    \"%d\"", minor_str, minor);
            changed = true;
        }
        // Revision.
        else if(CoreString::Contains(line, revision_str))
        {
            line    = CoreString::Format("%s \"%d\"", revision_str, revision);
            changed = true;
        }

        if(changed)
        {
            if(range.first == 0) range.first  = i;
            else                 range.second = i;
        }
    }

    return range;
}

std::pair<int, int> replace_doxy(
    const std::string        &projectName,
    std::vector<std::string> *pContents,
    int                       major,
    int                       minor,
    int                       revision) noexcept
{
    auto range = std::pair<int, int>();

    if(pContents->empty())
        return range;

    auto kVersion_Str = "PROJECT_NUMBER         =";
    for(int i = 0; i < pContents->size(); ++i)
    {
        auto &line   = (*pContents)[i];
        if(CoreString::Contains(line, kVersion_Str))
        {
            line = CoreString::Format(
                "%s v%d.%d.%d",
                kVersion_Str,
                major,
                minor,
                revision
            );

            range.first = range.second = i;
            break;
        }
    }

    return range;
}

//------------------------------------------------------------------------------
// Others...
void print_range(
    const std::vector<std::string> &contents,
    const std::pair<int,int>       &range) noexcept
{
    if(contents.empty())
        return;

    for(int i = range.first; i <= range.second; ++i)
        std::cout << contents[i] << "\n";

    std::cout << std::endl;
}

void split_version_numbers(
    const std::string &versionStr,
    int *pMajor_Out,
    int *pMinor_Out,
    int *pRevision_Out) noexcept
{
    auto result = sscanf(
        versionStr.c_str(),
        "%i.%i.%i",
        pMajor_Out,
        pMinor_Out,
        pRevision_Out);

    if(result != 3)
        show_error("Version isn't in format or Major.Minor.Revisio");
}




//----------------------------------------------------------------------------//
// Entry Point                                                                //
//----------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
    if(argc < 4) //1 prog-name, 3 required args, 1 optional arg.
        show_usage();

    //--------------------------------------------------------------------------
    // Required args.
    std::string project_name = argv[1];
    std::string version      = argv[2];
    std::string header_file  = argv[3];

    //--------------------------------------------------------------------------
    // Optional arg.
    std::string doxy_file     = "";
    bool        has_doxy_file = false;

    if(argc == 5)
    {
        has_doxy_file = true;
        doxy_file     = argv[4];
    }


    //--------------------------------------------------------------------------
    // Version numbers.
    int major, minor, revision;
    split_version_numbers(version, &major, &minor, &revision);


    //--------------------------------------------------------------------------
    // Log...
    std::cout << "-----------------------------------------------------------\n";
    std::cout << "Project Name: " << project_name << "\n";
    std::cout << "Version:      " << version      << "\n";
    std::cout << "Header File:  " << header_file  << "\n";
    std::cout << "Doxy File:    " << doxy_file    << "\n";

    std::cout << "-----------------------------------------------------------\n";
    std::cout << "MAJOR:    " << major    << "\n";
    std::cout << "MINOR:    " << minor    << "\n";
    std::cout << "REVISION: " << revision << "\n";


    //--------------------------------------------------------------------------
    // Sanity Checks...
    if(!CoreFS::IsFile(header_file))
        show_error("Header file doesn't exists.");

    if(has_doxy_file && !CoreFS::IsFile(doxy_file))
        show_error("Doxy file doesn't exits.");


    //--------------------------------------------------------------------------
    // Replace
    auto header_contents = read_file(header_file);
    auto doxy_contents   = read_file(doxy_file  );

    auto header_range = replace_header(
        project_name,
        &header_contents,
        major,
        minor,
        revision
    );

    auto doxy_range = replace_doxy(
        project_name,
        &doxy_contents,
        major,
        minor,
        revision
    );


    //--------------------------------------------------------------------------
    // Prompt.
    std::cout << "-----------------------------------------------------------\n";
    print_range(header_contents, header_range);
    print_range(doxy_contents,   doxy_range  );
    std::cout << "\nIs this correct? [y/N]";

    char reply;
    std::cin >> reply;
    if(reply != 'y' && reply != 'Y')
        exit(0);


    //--------------------------------------------------------------------------
    // Write.
    write_file(header_contents, header_file);
    write_file(doxy_contents,   doxy_file  );


    //--------------------------------------------------------------------------
    // Done..
    std::cout << "Done..." << "\n";
}
