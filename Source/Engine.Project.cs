using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("../Sharpmake/Base.Project.cs")]
[module: Sharpmake.Include("ThirdParty/SDL2/SDL2.Library.cs")]

[Generate]
class EngineProject : BaseProject
{
    public EngineProject() : base("Engine") {}

    public override void Configure(Configuration conf, Target target)
    {
        base.Configure(conf, target);

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]/Engine");
        
        conf.AddPublicDependency<SDL2Library>(target);
    }
}
