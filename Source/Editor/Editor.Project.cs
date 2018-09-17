using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Export]
class EditorProject : BaseProject
{
    public EditorProject() : base("Editor") {}
    
    public override void ConfigureAll(Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);
        
        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]");

        conf.AddPublicDependency<EngineProject>(target);
    }
}
