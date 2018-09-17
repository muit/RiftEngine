using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("Properties/Base.Project.cs")]
[module: Sharpmake.Include("Source/Engine/Engine.Project.cs")]
[module: Sharpmake.Include("Source/Game/Game.Project.cs")]
[module: Sharpmake.Include("Source/Editor/Editor.Project.cs")]
[module: Sharpmake.Include("Source/ThirdParty/SDL2/SDL2.Library.cs")]

[Generate]
class FecoEngineSolution : Solution
{
    public FecoEngineSolution()
    {
        // The name of the solution.
        Name = "FecoEngine";
        IsFileNameToLower = false;

        // As with the project, define which target this solution builds for.
        // It's usually the same thing.
        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2017,
            Optimization.Debug | Optimization.Release
        ));
    }

    [Configure()]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {   
        // Puts the generated solution in the /generated folder too.
        conf.SolutionPath = @"[solution.SharpmakeCsPath]";

        conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP14);

        if(target.Optimization == Optimization.Release)
        {
            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }

        conf.AddProject<EngineProject>(target);
        conf.AddProject<GameProject>(target);
        conf.AddProject<EditorProject>(target);
        conf.AddProject<SDL2Library>(target);
    }


    [Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<FecoEngineSolution>();
    }
}
