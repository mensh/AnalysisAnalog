using System;
using System.Globalization;
using System.Threading;
using System.Windows.Forms;

namespace AnalysisAnalog
{
    public partial class AddParametr : DevExpress.XtraEditors.XtraForm
    {
        readonly BindingSource _bindingSource = new BindingSource();
        public AddParametr(BindingSource _bindingSource)
        {
            InitializeComponent();
            if (_bindingSource != null)
                this._bindingSource = _bindingSource;

        }

        private void SimpleButton1_Click(object sender, EventArgs e)
        {
            _bindingSource.Add(AddNewParametr());
        }

        private Form1.Analysis AddNewParametr()
        {
            Form1.Analysis analysis = new Form1.Analysis();
            analysis.SizeArray = (int)spinCountArray.Value;
            analysis.Name = textEditName.Text;
            analysis.Cmr = ConvertToDouble(spinEditCMR.Value.ToString(CultureInfo.InvariantCulture));
            analysis.Mask = textEditMask.Text.Contains("0x") ? Convert.ToInt32(textEditMask.Text, 16) : Convert.ToInt32(textEditMask.Text);
            analysis.Address = textEditAddress.Text.Contains("0x") ? Convert.ToInt32(textEditAddress.Text, 16) : Convert.ToInt32(textEditAddress.Text);
            return analysis;
        }





        private double ConvertToDouble(string s)
        {
            char systemSeparator = Thread.CurrentThread.CurrentCulture.NumberFormat.CurrencyDecimalSeparator[0];
            double result = 0;
            try
            {
                if (s != null)
                    if (!s.Contains(","))
                        result = double.Parse(s, CultureInfo.InvariantCulture);
                    else
                        result = Convert.ToDouble(s.Replace(".", systemSeparator.ToString()).Replace(",", systemSeparator.ToString()));
            }
            catch (Exception e)
            {
                try
                {
                    result = Convert.ToDouble(s);
                }
                catch
                {
                    try
                    {
                        result = Convert.ToDouble(s.Replace(",", ";").Replace(".", ",").Replace(";", "."));
                    }
                    catch {
                        throw new Exception("Wrong string-to-double format");
                    }
                }
            }
            return result;
        }

        private Form1.Analysis current_parametr;
        public Form1.Analysis ShowParametr(Form1.Analysis parametr)
        {
            current_parametr = parametr;
            textEditName.Text = parametr.Name;
            spinCountArray.Value = parametr.SizeArray;
            spinEditCMR.Value = (decimal)parametr.Cmr;
            textEditAddress.Text = parametr.Address.ToString();
            textEditMask.Text = parametr.Mask.ToString();
            return parametr;
        }

        private void SimpleButton2_Click(object sender, EventArgs e)
        {
            _bindingSource.Remove(current_parametr);
            _bindingSource.Add(AddNewParametr());
        }
    }
}