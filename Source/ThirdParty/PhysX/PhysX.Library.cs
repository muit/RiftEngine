using Sharpmake;

[Generate]
public class PhysXLibrary : Project
{
    public string BasePath = @"[project.SharpmakeCsPath]/include";

    public PhysXLibrary() : base(typeof(RiftTarget))
    {
        Name = "PhysX";

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
        conf.ProjectPath = "[project.SharpmakeCsPath]/../../../Binaries/PhysX";

        conf.TargetLibraryPath = "[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]";
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysX.dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXCommon.dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXCooking.dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXFoundation.dll");

		conf.LibraryFiles.Add(@"PhysX.lib");
		conf.LibraryFiles.Add(@"PhysXCommon.lib");
		conf.LibraryFiles.Add(@"PhysXCooking.lib");
		conf.LibraryFiles.Add(@"PhysXFoundation.lib");

        // To use /DEBUG:FULL
        conf.Options.Add(Sharpmake.Options.Vc.Linker.GenerateDebugInformation.Enable);
        conf.Options.Add(Sharpmake.Options.Vc.Linker.GenerateFullProgramDatabaseFile.Enable);

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
