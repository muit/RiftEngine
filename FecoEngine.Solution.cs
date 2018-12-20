using System;
using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("Properties/Base.Project.cs")]
[module: Sharpmake.Include("Properties/Base.Module.cs")]
[module: Sharpmake.Include("Source/Engine/Engine.Module.cs")]
[module: Sharpmake.Include("Source/Game/Game.Project.cs")]
[module: Sharpmake.Include("Source/ThirdParty/SDL2/SDL2.Library.cs")]

[Generate]
class FecoEngineSolution : Solution
{
    public FecoEngineSolution() : base(typeof(FecoTarget))
    {
        // The name of the solution.
        Name = "FecoEngine";
        IsFileNameToLower = false;

        // As with the project, define which target this solution builds for.
        // It's usually the same thing.
        AddTargets(new FecoTarget( Editor.Editor | Editor.Game ));
    }

    [Configure()]
    public void ConfigureAll(Solution.Configuration conf, FecoTarget target)
    {   
        // Puts the generated solution in the /generated folder too.
        conf.SolutionPath = @"[solution.SharpmakeCsPath]";
        //conf.ProjectPath = @"[solution.SharpmakeCsPath]/Binaries";

        conf.AddProject<GameProject>(target);
        conf.AddProject<EngineModule>(target);
        conf.AddProject<SDL2Library>(target);
    }


    [Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<FecoEngineSolution>();
    }
}


[Fragment, Flags]
public enum Editor
{
    Editor = 0x01,
    Game = 0x02
}

class FecoTarget : Target {
    public Editor Editor;

    public override string Name
    {
        get {
            //if(Optimization == Optimization.Release)
            //    return Optimization.ToString();
            return Optimization.ToString() + " " + Editor.ToString();
        }
    }

    public FecoTarget() : base() { }

    public FecoTarget(Editor editor, OutputType outputType = OutputType.Lib) : base(
        Platform.win32 | Platform.win64,
        DevEnv.vs2017,
        Optimization.Debug | Optimization.Release,
        outputType)
    {
        Editor = editor;
    }
}
