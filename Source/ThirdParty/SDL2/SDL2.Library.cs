using Sharpmake;

[Generate]
public class SDL2Library : Project
{
    public string BasePath = @"[project.SharpmakeCsPath]/include";

    public SDL2Library() : base(typeof(RiftTarget))
    {
        Name = "SDL2";

        AddTargets(new RiftTarget(
            Editor.Editor | Editor.Game,
            OutputType.Dll | OutputType.Lib
        ));

        SourceRootPath = "[project.BasePath]";

        IsFileNameToLower = false;
    }

    [Configure()]
    public void ConfigureAll(Configuration conf, Target target)
    {
        conf.SolutionFolder = "RiftEngine/ThirdParty";
        conf.IncludePaths.Add("[project.BasePath]");
        conf.ProjectPath = "[project.SharpmakeCsPath]/../../../Binaries/SDL2";
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
