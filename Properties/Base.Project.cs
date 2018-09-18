using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

class BaseProject : Project
{
    public BaseProject() {}
    public BaseProject(string CustomName)
    {
        Name = CustomName;
        SourceRootPath = @"[project.SharpmakeCsPath]";
        IsFileNameToLower = false;

        AddTargets(new Target(
            Platform.win32 | Platform.win64,
            DevEnv.vs2017,
            Optimization.Debug | Optimization.Release
        ));
    }

    [Configure()]
    public virtual void ConfigureAll(Configuration conf, Target target)
    {
        conf.ProjectPath = @"[project.SharpmakeCsPath]\..\..\Projects\[project.Name]";conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP14);
        
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP14);
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.RTTI.Enable);
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Enable);

        if(target.Optimization == Optimization.Release)
        {
            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    
    }
}
