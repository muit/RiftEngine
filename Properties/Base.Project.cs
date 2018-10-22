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


        conf.Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Enable);

        if(target.Optimization == Optimization.Release)
        {
            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
        else if(target.Optimization == Optimization.Debug)
        {
            conf.Options.Add(Sharpmake.Options.Vc.Compiler.Optimization.Disable);
            conf.Options.Add(Sharpmake.Options.Vc.Compiler.Inline.OnlyInline);
        }
    
        if(target.Editor == Editor.Editor)
        {
            conf.Defines.Add("WITH_EDITOR");
        }
    }
}
