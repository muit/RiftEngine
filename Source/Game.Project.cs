using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("../Sharpmake/Base.Project.cs")]
[module: Sharpmake.Include("Engine.Project.cs")]

[Generate]
class GameProject : BaseProject
{
    public GameProject() : base("Game") {}

    public override void Configure(Configuration conf, Target target)
    {
        base.Configure(conf, target);
        conf.SolutionFolder = "Game";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/Game");
        
        conf.AddPublicDependency<EngineProject>(target);
    }
}
