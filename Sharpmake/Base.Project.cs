using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Generate]
class BaseProject : Project
{
    public BaseProject(string CustomName)
    {
        Name = CustomName;
        SourceRootPath = @"[project.SharpmakeCsPath]/[project.Name]";
        IsFileNameToLower = false;

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2017,
            Optimization.Debug | Optimization.Release
        ));
    }

    [Configure()]
    public virtual void Configure(Configuration conf, Target target)
    {
        conf.SolutionFolder = "CrabEngine";
        conf.ProjectPath = @"[project.SharpmakeCsPath]\..\Projects\[project.Name]";
    }
}
