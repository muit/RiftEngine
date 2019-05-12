using System;
using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[module: Sharpmake.Include("Properties/Base.Project.cs")]
[module: Sharpmake.Include("Properties/Base.Module.cs")]
[module: Sharpmake.Include("Source/Engine/Engine.Module.cs")]
[module: Sharpmake.Include("Source/Game/Game.Project.cs")]
[module: Sharpmake.Include("Source/Test/Test.Project.cs")]
[module: Sharpmake.Include("Source/ThirdParty/SDL2/SDL2.Library.cs")]
[module: Sharpmake.Include("Source/ThirdParty/Assimp/Assimp.Library.cs")]
[module: Sharpmake.Include("Source/ThirdParty/PhysX/PhysX.Library.cs")]
[module: Sharpmake.Include("Source/ThirdParty/GoogleTest/GoogleTest.Library.cs")]

[Generate]
class RiftEngineSolution : Solution
{
    public RiftEngineSolution() : base(typeof(RiftTarget))
    {
        // The name of the solution.
        Name = "RiftEngine";
        IsFileNameToLower = false;

        // As with the project, define which target this solution builds for.
        // It's usually the same thing.
        AddTargets(new RiftTarget( Editor.Editor | Editor.Game ));
    }

    [Configure]
    public virtual void ConfigureAll(Solution.Configuration conf, RiftTarget target)
    {   
        // Puts the generated solution in the /generated folder too.
        conf.SolutionPath = @"[solution.SharpmakeCsPath]";
        //conf.ProjectPath = @"[solution.SharpmakeCsPath]/Binaries";

        conf.AddProject<GameProject>(target);
        conf.AddProject<TestProject>(target);
        conf.AddProject<EngineModule>(target);
        conf.AddProject<SDL2Library>(target);
        conf.AddProject<AssimpLibrary>(target);
        conf.AddProject<PhysXLibrary>(target);
    }


    [Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<RiftEngineSolution>();
    }
}


[Fragment, Flags]
public enum Editor
{
    Editor = 0x01,
    Game = 0x02
}

class RiftTarget : Target {
    public Editor Editor;

    public override string Name
    {
        get {
            //if(Optimization == Optimization.Release)
            //    return Optimization.ToString();
            return Optimization.ToString() + " " + Editor.ToString();
        }
    }

    public RiftTarget() { }

    public RiftTarget(Editor editor, OutputType outputType = OutputType.Lib) : base(
        Platform.win32 | Platform.win64,
        DevEnv.vs2017,
        Optimization.Debug | Optimization.Release,
        outputType)
    {
        Editor = editor;
    }
}
