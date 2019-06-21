using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using DevExpress.UserSkins;
using DevExpress.Skins;
using DevExpress.LookAndFeel;

namespace AnalysisAnalog
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            EngageBackgroundJit();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            BonusSkins.Register();
            Application.Run(new Form1());
        }

        private static void EngageBackgroundJit()
        {
            var appPath = new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath;
            var appFolder = System.IO.Path.GetDirectoryName(appPath);
            var appName = System.IO.Path.GetFileName(appPath);
            System.Runtime.ProfileOptimization.SetProfileRoot(appFolder);
            System.Runtime.ProfileOptimization.StartProfile(appName + ".profile");
        }
    }
}
