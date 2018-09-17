using Sharpmake;

[Sharpmake.Generate]
public class SDL2Library : Project
{
    public string BasePath = @"[project.SharpmakeCsPath]/include";

    public SDL2Library()
    {
        Name = "SDL2";

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2017,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll | OutputType.Lib
        ));

        SourceRootPath = "[project.BasePath]";

        IsFileNameToLower = false;
    }

    [Configure()]
    public void Configure(Configuration conf, Target target)
    {
        conf.SolutionFolder = "FecoEngine/ThirdParty";
        conf.IncludePaths.Add("[project.BasePath]");
        conf.ProjectPath = "[project.SharpmakeCsPath]/../../../Projects/SDL2";
        conf.PrecompHeader = "SDL.h";

        conf.TargetLibraryPath = "[project.SharpmakeCsPath]/lib/[target.Platform]";
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/SDL2.dll");

		conf.LibraryFiles.Add(@"SDL2.lib");
		conf.LibraryFiles.Add(@"SDL2main.lib");


        if (target.OutputType == OutputType.Dll)
        {
            conf.Output = Configuration.OutputType.Dll;
            conf.Defines.Add("_DLL");
        }
        else if (target.OutputType == OutputType.Lib)
        {
            conf.Output = Configuration.OutputType.Lib;
        }
    }
}
