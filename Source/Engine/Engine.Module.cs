using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Generate]
class EngineModule : BaseModule
{
    public EngineModule() : base("Engine") {}

    public override void ConfigureAll(Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);
        conf.SolutionFolder = "FecoEngine";

        conf.AddPublicDependency<SDL2Library>(target);
    }
}
