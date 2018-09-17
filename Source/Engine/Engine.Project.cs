using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

//[module: Sharpmake.Include("../ThirdParty/SDL2/SDL2.Library.cs")]

[Generate]
class EngineProject : BaseProject
{
    public EngineProject() : base("Engine") {

        AddFragmentMask(OutputType.Dll | OutputType.Lib);

    }

    public override void ConfigureAll(Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);
        conf.SolutionFolder = "FecoEngine";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]");

        conf.AddPublicDependency<SDL2Library>(target);
        
        if (target.OutputType == OutputType.Dll)
        {
            conf.Output = Configuration.OutputType.Dll;
        }
        else if (target.OutputType == OutputType.Lib)
        {
            conf.Output = Configuration.OutputType.Lib;
        }
    }
}
