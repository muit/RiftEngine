using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

//[module: Sharpmake.Include("../ThirdParty/SDL2/SDL2.Library.cs")]

[Export]
class EngineProject : BaseProject
{
    public EngineProject() : base("Engine") {}

    public override void ConfigureAll(Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);
        conf.SolutionFolder = "FecoEngine";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]\World");
        
        conf.AddPublicDependency<SDL2Library>(target);
    }
}
