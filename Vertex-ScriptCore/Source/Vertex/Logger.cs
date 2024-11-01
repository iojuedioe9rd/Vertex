using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{


    public static class Logger
    {
        public const int CORE_LOG_FLAG = 1 << 0;    // 0b000001 = 1
        public const int LOG_TRACE     = 1 << 1;    // 0b000010 = 2
        public const int LOG_INFO      = 1 << 2;    // 0b000100 = 4
        public const int LOG_WARN      = 1 << 3;    // 0b001000 = 8
        public const int LOG_ERROR     = 1 << 4;    // 0b010000 = 16
        public const int LOG_CRITICAL  = 1 << 5;    // 0b100000 = 32

        public static AppLogger appLogger = new AppLogger();
        public static CoreLogger coreLogger = new CoreLogger();
        public static BaseLogger logger = coreLogger;

        public static void Trace(params object[] args)
        {
            string[] strings = new string[args.Length];

            for (int i = 0; i < args.Length; i++)
            {
                strings[i] = args[i].ToString();
            }

            logger.Trace(strings);
        }

        public static void Info(params object[] args)
        {
            string[] strings = new string[args.Length];

            for (int i = 0; i < args.Length; i++)
            {
                strings[i] = args[i].ToString();
            }

            logger.Info(strings);
        }

        public static void Warn(params object[] args)
        {
            string[] strings = new string[args.Length];

            for (int i = 0; i < args.Length; i++)
            {
                strings[i] = args[i].ToString();
            }

            logger.Warn(strings);
        }

        public static void Error(params object[] args)
        {
            string[] strings = new string[args.Length];

            for (int i = 0; i < args.Length; i++)
            {
                strings[i] = args[i].ToString();
            }

            logger.Error(strings);
        }

        public static void Critical(params object[] args)
        {
            string[] strings = new string[args.Length];

            for (int i = 0; i < args.Length; i++)
            {
                strings[i] = args[i].ToString();
            }

            logger.Critical(strings);
        }
    }
    
    public abstract class BaseLogger
    {
        public abstract void Trace(params string[] objects);
        public abstract void Info(params string[] objects);
        public abstract void Warn(params string[] objects);
        public abstract void Error(params string[] objects);
        public abstract void Critical(params string[] objects);
    }

    public class AppLogger : BaseLogger
    {
        public override void Trace(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_TRACE);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Info(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_INFO);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Warn(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_WARN);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Error(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_ERROR);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Critical(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_CRITICAL);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }
    }

    public class CoreLogger : BaseLogger
    {
        public override void Trace(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_TRACE | Logger.CORE_LOG_FLAG);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Info(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_INFO | Logger.CORE_LOG_FLAG);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Warn(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_WARN | Logger.CORE_LOG_FLAG);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Error(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_ERROR | Logger.CORE_LOG_FLAG);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }

        public override void Critical(params string[] objects)
        {
            InternalCalls.BeginLog(Logger.LOG_CRITICAL | Logger.CORE_LOG_FLAG);

            for (int i = 0; i < objects.Length; i++)
            {
                InternalCalls.LogMsg(objects[i]);
            }

            InternalCalls.EndLog();
        }
    }
}
