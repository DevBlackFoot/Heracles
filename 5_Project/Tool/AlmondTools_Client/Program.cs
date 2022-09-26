namespace AlmondTools_Client
{
    using Forms;
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();            

            FormManager formManager = new FormManager();
            formManager.DefaultForm.Width = 1680;
            formManager.DefaultForm.Height = 790;    
            

            Application.Run(formManager.DefaultForm);
        }
    }
}