using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using RzReciver;

namespace AnalysisAnalog
{
    public sealed class ClassRzReciverNet : IDisposable
    {
        private ResultReciveTransmitter _resultRecive;
        private readonly ClassRzReciver _rzReciver = new ClassRzReciver();
        public bool Connect = false;
        public class ResultReciveTransmitter
        {
            public List<int> RzMessage = new List<int>();
            public long TimeStart;
            public long TimerEnd;
            public long TimerCounter;
            public List<int> Page;
            public int Numfile = 1;
            public int Numpage = 0;
            public int Startblock = 0;
            public bool Writeprotacol;
        }

        public BackgroundWorker BackgroundWorkerReadRz;
        private readonly CheckBox _radToggleButtonElementRec = new CheckBox();
        public delegate void ReadComlte(ResultReciveTransmitter resultRecive);
        public event ReadComlte Withdrawn;

        public delegate void ChangeStatus(string status);

        public event ChangeStatus ChangStatus;

        public void Rec(bool rec)
        {
            _radToggleButtonElementRec.Checked = rec;
        }

        public bool IsConnect()
        {
            return ClassRzReciver.RzUsb_IsConnectToDeviceC();
        }

        public void RZ_run()
        {
            var result = Init_rz_adapter();
            if (result)
            {
                BackgroundWorkerReadRz = new BackgroundWorker();
                BackgroundWorkerReadRz.DoWork += BackgroundWorker_ReadRZ_DoWork;
                _resultRecive = new ResultReciveTransmitter();
                BackgroundWorkerReadRz.RunWorkerCompleted += BackgroundWorker_ReadRZ_RunWorkerCompleted;
                if (BackgroundWorkerReadRz.IsBusy != true) BackgroundWorkerReadRz.RunWorkerAsync();
            }
        }

        private void BackgroundWorker_ReadRZ_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (Withdrawn != null)
            {
                foreach (Delegate d in Withdrawn.GetInvocationList())
                {
                    d.DynamicInvoke(_resultRecive);
                }
            }

            //  Withdrawn?.Invoke(_resultRecive);
        }

        private void BackgroundWorker_ReadRZ_DoWork(object sender, DoWorkEventArgs e)
        {
                List<int> rzMessage = _rzReciver.RzUsb_ReceiveRZStreamExC();
          
                _resultRecive.RzMessage.Clear();
                Stopwatch timer = new Stopwatch();
                if (_radToggleButtonElementRec.Checked)
                {
                    timer.Start();
                    _resultRecive.TimeStart = _resultRecive.TimerEnd;
                }

                _resultRecive.RzMessage = rzMessage;

                if (_radToggleButtonElementRec.Checked)
                {
                    long timeTick = 0xffff * 2;
                    _resultRecive.TimerCounter = _resultRecive.TimerCounter + (timeTick / 0xffff);
                    _resultRecive.TimerEnd = (0xffff - (timeTick / 0xffff));
                    timer.Stop();
                }

                e.Result = _resultRecive;
                Thread.Sleep(5);
        }

        private string Disconect_rz_adapter()
        {
               var result = ClassRzReciver.RzUsb_StopReceiveRZC();
                var str = result ? "Отключено" : "Ошибка отключения";
                ClassRzReciver.RzUsb_DisconnectDeviceC();
                return str;
        }

        private bool Init_rz_adapter()
        {
            if (ClassRzReciver.RzUsb_ConnectDeviceC())
            {

                ChangStatus?.Invoke("Обнаружен Адаптер");
            }
            else
            {
                ChangStatus?.Invoke("Адаптер НЕ обнаружен");
                Disconect_rz_adapter();
                return false;
            }

            if (ClassRzReciver.RzUsb_IsConnectToDeviceC())
            {
                ChangStatus?.Invoke("Подключен к адаптер");
            }
            else
            {
                ChangStatus?.Invoke("Ошибка подключения к адаптеру");
                Disconect_rz_adapter();
                return false;
            }

        
            if (!ClassRzReciver.RzUsb_InitDeviceExC())
            {
                ChangStatus?.Invoke("Oшибка инициализации адаптера");
                Disconect_rz_adapter();
                return false;
            }

            //организация приёмаRZ-потока
            //запуск приёмаRZ-потока
            if (!ClassRzReciver.RzUsb_RunReceiveRZC())
            {
                ChangStatus?.Invoke("Oшибка запуска приёма RZ потока");
                return false;
            }

            Connect = true;
            return true;
        }

        #region IDisposable Support
        private bool _disposedValue; // Для определения избыточных вызовов

        private void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                  BackgroundWorkerReadRz?.Dispose();
                  _radToggleButtonElementRec.Dispose();
                }

                // TODO: освободить неуправляемые ресурсы (неуправляемые объекты) и переопределить ниже метод завершения.
                // TODO: задать большим полям значение NULL.

                _disposedValue = true;
            }
        }

        // TODO: переопределить метод завершения, только если Dispose(bool disposing) выше включает код для освобождения неуправляемых ресурсов.
        // ~ClassRzReciverNet() {
        //   // Не изменяйте этот код. Разместите код очистки выше, в методе Dispose(bool disposing).
        //   Dispose(false);
        // }

        // Этот код добавлен для правильной реализации шаблона высвобождаемого класса.
        public void Dispose()
        {
            // Не изменяйте этот код. Разместите код очистки выше, в методе Dispose(bool disposing).
            Dispose(true);
            // TODO: раскомментировать следующую строку, если метод завершения переопределен выше.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
