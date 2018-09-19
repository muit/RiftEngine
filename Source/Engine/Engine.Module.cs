using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Generate]
class EngineModule : BaseModule
{
    public EngineModule() : base("Engine") {}

    public override void ConfigureAll(Configuration conf, FecoTarget target)
    {
        base.ConfigureAll(conf, target);

        conf.AddPublicDependency<SDL2Library>(target);

        conf.SolutionFolder = "FecoEngine";
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/ThirdParty");
        
        conf.Options.Add(new Sharpmake.Options.Vc.Compiler.DisableSpecificWarnings("4152"));

        switch(target.Platform) {
            case Platform.win32:
                conf.Defines.Add("PLATFORM_WINDOWS");
                conf.Defines.Add("_WIN32");
                conf.ExportDefines.Add("PLATFORM_WINDOWS");
                conf.ExportDefines.Add("_WIN32");
                break;
            case Platform.win64:
                conf.Defines.Add("PLATFORM_WINDOWS");
                conf.Defines.Add("_WIN64");
                conf.ExportDefines.Add("PLATFORM_WINDOWS");
                conf.ExportDefines.Add("_WIN64");
                break;
            default:
                break;
        }
    }
}
