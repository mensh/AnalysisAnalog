﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;
using AnalysisAnalog.Annotations;
using DevExpress.Data;
using DevExpress.XtraGrid.Views.Grid;

namespace AnalysisAnalog
{
    public partial class Form1 : DevExpress.XtraEditors.XtraForm
    {
        private readonly ClassRzReciverNet _rzreciver = new ClassRzReciverNet();
        private BackgroundWorker _backgroundsearchAndWrite;
        private DateTime _date1;
        private Stopwatch _timer;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        [Serializable]
        public sealed class Analysis : INotifyPropertyChanged
        {

            public Analysis()
            {
                _ArrayFizika.CollectionChanged += _ArrayFizika_CollectionChanged;
            }

            private void _ArrayFizika_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
            {
                OnPropertyChanged();
            }

            private ObservableCollection<double> _ArrayFizika = new ObservableCollection<double>();

            [Display(GroupName = "<Name|>", Name = "Точки массива")]
            public ObservableCollection<double> ArrayFizika
            {
                get => _ArrayFizika;
                set
                {
                    if (value != null)
                    {
                        _ArrayFizika = value;
                        OnPropertyChanged();
                    }
                }
            }


            private string _Name;
            [Display(GroupName = "<Name|>", Name = "Имя параметра")]
            public string Name
            {
                get => _Name;
                set
                {
                    if (value != _Name)
                    {
                        _Name = value;
                        OnPropertyChanged();
                    }
                }
            }

            private int _Address;
            [Display(GroupName = "<Name|>", Name = "Адресс")]
            public int Address
            {
                get => _Address;
                set
                {
                    if (value != _Address)
                    {
                        _Address = value;
                        OnPropertyChanged();
                    }
                }
            }

            private int _Mask;
            [Display(GroupName = "<Name|>", Name = "Маска")]
            public int Mask    
            {
                get => _Mask;
                set
                {
                    if (value != _Mask)
                    {
                        _Mask = value;
                        OnPropertyChanged();
                    }
                }
            }
            private double _CMR;
            [Display(GroupName = "<Name|>", Name = "ЦМР")]
            public double Cmr
            {
                get => _CMR;
                set
                {
                    if (Math.Abs(value - _CMR) > TOLERANCE)
                    {
                        _CMR = value;
                        OnPropertyChanged();
                    }
                }
            }
            private double _Dispersy;
            [Display(GroupName = "<Name|>", Name = "Дисперсия")]
            public double Dispersy
            {
                get => _Dispersy;
                set
                {
                    if (Math.Abs(value - _Dispersy) > TOLERANCE)
                    {
                        _Dispersy = value;
                        OnPropertyChanged();
                    }
                }
            }

            private double _DispersyPersent;
            [Display(GroupName = "<Name|>", Name = "Дисперсия %")]
            public double DispersyPersent
            {
                get => _DispersyPersent;
                set
                {
                    if (Math.Abs(value - _DispersyPersent) > TOLERANCE)
                    {
                        _DispersyPersent = value;
                        OnPropertyChanged();
                    }
                }
            }

            private double _Srednie;
            [Display(GroupName = "<Name|>", Name = "Среднее")]
            public double Srednie
            {
                get => _Srednie;
                set
                {
                    if (Math.Abs(value - _Srednie) > TOLERANCE)
                    {
                        _Srednie = value;
                        OnPropertyChanged();
                    }
                }
            }

            private double _Fizika;
            [Display(GroupName = "<Name|>", Name = "Текущие ФИЗИКА")]
            public double Fizika
            {
                get => _Fizika;
                set
                {
                    if (Math.Abs(value - _Fizika) > TOLERANCE)
                    {
                        _Fizika = value;
                        OnPropertyChanged();
                    }
                }
            }
          
            private int _SizeArray;  
            [Display(GroupName = "<Name|>", Name = "Размер массива")]
            public int SizeArray
            {
                get => _SizeArray;
                set
                {
                    if (value != _SizeArray)
                    {
                        _SizeArray = value;
                        OnPropertyChanged();
                    }
                }
            }

            private int _Counter;
            private double TOLERANCE = 0.001;

            [Display(GroupName = "<Name|>", Name = "Счетчик")]
            public int Counter
            {
                get => _Counter;
                set
                {
                    if (value != _Counter)
                    {
                        _Counter = value;
                        OnPropertyChanged();
                    }
                }
            }


            public event PropertyChangedEventHandler PropertyChanged;

            [NotifyPropertyChangedInvocator]
            private void OnPropertyChanged([CallerMemberName] string propertyName = null)
            {
              //  PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
                  var handler = PropertyChanged;
                  if (handler != null)
                  {
                      if (Application.OpenForms.Count == 0) return;
                      var mainForm = Application.OpenForms[0];
                      if (mainForm == null) return; // No main form - no calls

                      if (mainForm.InvokeRequired)
                      {
                          // We are not in UI Thread now
                          mainForm.Invoke(handler, new object[]
                          {
                              this, new PropertyChangedEventArgs(propertyName)
                          });
                      }
                      else
                      {
                          handler(this, new PropertyChangedEventArgs(propertyName));
                      }
                  }
            }
        }

        public Form1()
        {
            InitializeComponent();
            _rzreciver.ChangStatus += _rzreciver_ChangStatus;
            CurrencyDataController.DisableThreadingProblemsDetection = true;
        }

        private void _rzreciver_ChangStatus(string status)
        {
            barStaticItem1.Caption = status;
        }

        private void WriteStructToFile(string filepatch)
        {
            string filename = filepatch;
            using var fileWrite = File.OpenWrite(filename);
            var writer = new BinaryFormatter();
            writer.Serialize(fileWrite, analysisBindingSource.List); // Writes the entire list.
        }




        private void RzReciverRun()
        {
            _date1 = DateTime.Now;
            if (_rzreciver != null && _rzreciver.Connect ==false)
            {
                // _rzreciver = new ClassRzReciverNet();
                _rzreciver.RZ_run();
                _rzreciver.Withdrawn += _rzreciver_Withdrawn;
                _rzreciver.ChangStatus += _rzreciver_ChangStatus;
            }
        }

        private void _rzreciver_Withdrawn(ClassRzReciverNet.ResultReciveTransmitter resultRecive)
        {
            _backgroundsearchAndWrite = new BackgroundWorker();
            _backgroundsearchAndWrite.DoWork += _backgroundsearchAndWrite_DoWork;
            _backgroundsearchAndWrite.RunWorkerAsync(resultRecive);
            _backgroundsearchAndWrite.RunWorkerCompleted += _backgroundsearchAndWrite_RunWorkerCompleted;
        }

        private void _backgroundsearchAndWrite_DoWork(object sender, DoWorkEventArgs e)
        {
            ReadMessageRzsss((ClassRzReciverNet.ResultReciveTransmitter)e.Argument);
        }

        private void _backgroundsearchAndWrite_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (chRUNStop.Checked)
            {
                if (_rzreciver?.BackgroundWorkerReadRz?.IsBusy != true)
                    _rzreciver?.BackgroundWorkerReadRz?.RunWorkerAsync();
            }
        }


        private void ReadMessageRzsss(ClassRzReciverNet.ResultReciveTransmitter resultRecive)
        {


            var rzMessage = new List<int>(resultRecive.RzMessage);
            try
            {
                    foreach (int t1 in rzMessage)
                    {
                        int address = t1 & 0x000000ff;
                        int data = (int)((t1 & 0xffffff00) >> 8);
                        //  _classUserData.bindingSourceActiveBinar.SuspendBinding();
          
                        //     _classUserData.bindingSourceActiveBinar.ResumeBinding();
                        //     _classUserData.bindingSourceActiveAnalog.SuspendBinding();
                        foreach (var gridViewRowInfo in analysisBindingSource.List.Cast<Analysis>())
                        {
                            var t = gridViewRowInfo;

                            if (t.Address == address)
                            {
                                //Разделитель коэффициентов формул
                  
                     
                        

                                var mask = t.Mask;
                                if (t.ArrayFizika.Count < t.SizeArray)
                                {
                                    t.ArrayFizika.Add(t.Fizika);
                                    t.Counter = t.ArrayFizika.Count;
                                }
                                else
                                {
                                    t.Srednie = t.ArrayFizika.Average();
                                    double sumOfSquaresOfDifferences = t.ArrayFizika.Select(val => (val - t.Srednie) * (val - t.Srednie)).Sum();
                                    t.Dispersy = Math.Sqrt(sumOfSquaresOfDifferences / t.ArrayFizika.Count);
                                    t.DispersyPersent = (t.Dispersy / t.Srednie) * 100;
                                    t.ArrayFizika.Clear();
                                }

                                t.Fizika = (Convert.ToDouble(BitConverter.ToInt16(BitConverter.GetBytes(data & mask), 0) * t.Cmr));
                                //_series0[gridViewRowInfo.Index] = t.Fizika;
                                //   t.Point.Y.Add(t.Fizika);
                                //  if (_timer != null) t.Point.X.Add(_date1.AddMilliseconds(value: _timer.Elapsed.Ticks / 10000));
                            }

                        }
                        //   _classUserData.bindingSourceActiveAnalog.ResumeBinding();
                    }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
               
            }
        
        }

        private void BtnRun_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            RzReciverRun();
        }

        private void ChRUNStop_CheckedChanged(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            if (chRUNStop.Checked)
            {
                if (_rzreciver?.BackgroundWorkerReadRz?.IsBusy != true)  _rzreciver?.BackgroundWorkerReadRz?.RunWorkerAsync();
            }
        }

        private void GridView1_DoubleClick(object sender, EventArgs e)
        {
            if (sender is GridView view)
            {
                Analysis row = ((Analysis)view.GetRow(view.FocusedRowHandle));
                if (row == null) return;
                AddParametr addParametr = new AddParametr(analysisBindingSource);
                addParametr.ShowParametr(row);
                addParametr.ShowDialog();
            }
        }

        private void BtnAddParametr_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            AddParametr addParametr = new AddParametr(analysisBindingSource);
            addParametr.ShowDialog();
        }

        private void BtnSave_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                WriteStructToFile(saveFileDialog1.FileName);
            }
        }

        private void ReadStructFromFile(string filepatch)
        {
            BindingList<Analysis> data;
            using (var file = File.OpenRead(filepatch))
            {
                var reader = new BinaryFormatter();
                data = (BindingList<Analysis>) reader.Deserialize(file); // Reads the entire list.
            }

            analysisBindingSource.Clear();
          

            foreach (var t in data)
            {
                analysisBindingSource.Add(t);
            }

        }

        private void BtnOpen_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                ReadStructFromFile(openFileDialog1.FileName);
            }
        }
    }
}