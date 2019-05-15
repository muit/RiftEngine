using System;
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
        String platformSuffix = target.Platform == Platform.win32? "_32" : "_64";

        conf.TargetFileFullName = @"[project.Name]" + platformSuffix;
        conf.SolutionFolder = "RiftEngine/ThirdParty";
        conf.IncludePaths.Add("[project.BasePath]");
        conf.ProjectPath = "[project.SharpmakeCsPath]/../../../Binaries/PhysX";

        conf.TargetLibraryPath = "[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]";
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysX" + platformSuffix + ".dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXCommon" + platformSuffix + ".dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXCooking" + platformSuffix + ".dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXFoundation" + platformSuffix + ".dll");
        conf.TargetCopyFiles.Add(@"[project.SharpmakeCsPath]/lib/[target.Platform]/[target.Optimization]/PhysXGpu" + platformSuffix + ".dll");

		conf.LibraryFiles.Add(@"PhysX" + platformSuffix + ".lib");
		conf.LibraryFiles.Add(@"PhysXCommon" + platformSuffix + ".lib");
		conf.LibraryFiles.Add(@"PhysXCooking" + platformSuffix + ".lib");
		conf.LibraryFiles.Add(@"PhysXFoundation" + platformSuffix + ".lib");

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
