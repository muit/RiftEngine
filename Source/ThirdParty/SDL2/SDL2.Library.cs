using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Generate]
class SDL2Library : Project
{
    public string BasePath = @"[project.SharpmakeCsPath]/sdl";

    public SDL2Library()
    {
        Name = "SDL2";
        SourceRootPath = @"[project.BasePath]";
        IsFileNameToLower = false;

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2017,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll | OutputType.Lib));
    }

    [Configure]
    public void ConfigureAll(Configuration conf, Target target)
    {
        // Folder inside the SLN
        conf.SolutionFolder = "FecoEngine/ThirdParty";

        conf.Name = @"[target.Optimization] [target.OutputType]";
        conf.IncludePaths.Add(@"[project.BasePath]/..");

        // Gives a unique path for the project because Visual Studio does not
        // like shared intermediate directories.
        conf.ProjectPath = Path.Combine("[project.SharpmakeCsPath]/../../../Projects/ThirdParty/[project.Name]");

        // Setup the precompiled headers for the project. Just assigning a
        // value to those fields is enough for Sharpmake to understand that
        // the project has precompiled headers.
        conf.PrecompHeader = "SDL.h";


        if(target.Platform == Platform.win32)
        {
            conf.TargetLibraryPath = "[project.SharpmakeCsPath]/lib/x86";
        }
        else if(target.Platform == Platform.win64)
        {
            conf.TargetLibraryPath = "[project.SharpmakeCsPath]/lib/x64";
        }

        if (target.OutputType == OutputType.Dll)
        {
            conf.Output = Configuration.OutputType.Dll;
            conf.ExportDefines.Add("_DLL");
            conf.Defines.Add("_DLL");
        }
        else if (target.OutputType == OutputType.Lib)
        {
            conf.Output = Configuration.OutputType.Lib;
        }
    }
}