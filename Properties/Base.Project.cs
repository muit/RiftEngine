using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library

class BaseProject : Project
{
    public BaseProject() : base(typeof(FecoTarget)) {}
    public BaseProject(string CustomName) : base(typeof(FecoTarget))
    {
        Name = CustomName;
        SourceRootPath = @"[project.SharpmakeCsPath]";
        IsFileNameToLower = false;

        AddTargets(new FecoTarget( Editor.Editor | Editor.Game ));
    }

    [Configure()]
    public virtual void ConfigureAll(Configuration conf, FecoTarget target)
    {
        conf.ProjectPath = @"[project.SharpmakeCsPath]\..\..\Projects\[project.Name]";conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP14);
        
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP14);
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.RTTI.Enable);
        conf.Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Enable);

        if(target.Optimization == Optimization.Release)
        {
            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    
        if(target.Editor == Editor.Editor && target.Optimization != Optimization.Release)
        {
            conf.Defines.Add("WITH_EDITOR");
        }
    }
}
