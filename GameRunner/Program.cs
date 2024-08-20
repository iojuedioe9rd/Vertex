using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;


namespace GameRunner
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Prepare the process to run
            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = "cmd.exe",
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = true,
                CreateNoWindow = true,
                Arguments = "/c game.exe"
            };
            int exitCode;

            Process proc = new Process {
                StartInfo = startInfo,
                
                EnableRaisingEvents = true
            };

            proc.OutputDataReceived += CmdProcess_OutputDataReceived;
            proc.ErrorDataReceived += CmdProcess_OutputDataReceived;

            proc.Start();
            proc.BeginOutputReadLine();
            proc.BeginErrorReadLine();

            using (proc)
            {

                proc.WaitForExit();

                // Retrieve the app's exit code
                exitCode = proc.ExitCode;
            }

            Console.WriteLine(exitCode.ToString());

        }

        private static void CmdProcess_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.WriteLine(e.Data);
        }
    }
}
