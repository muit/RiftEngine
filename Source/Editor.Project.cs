using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("../Sharpmake/Base.Project.cs")]
[module: Sharpmake.Include("Engine.Project.cs")]
[module: Sharpmake.Include("Game.Project.cs")]

[Generate]
class EditorProject : BaseProject
{
    public EditorProject() : base("Editor") {}
    
    public override void Configure(Configuration conf, Target target)
    {
        base.Configure(conf, target);
        
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/Editor");

        conf.AddPublicDependency<EngineProject>(target);
        conf.AddPublicDependency<GameProject>(target);
    }
}
