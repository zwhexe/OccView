using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Drawing;

namespace OccView
{
    public class OCCProxyer
    {
        public OCCProxy Proxy { get; private set; }

        public OCCProxyer()
        {
            Proxy = new OCCProxy();
        }

        public void MakeBox()
        {
            Proxy.occModel.MakeBox();
            Proxy.Display();
        }

        public void MakeCone()
        {
            Proxy.MakeOctShape();
            Proxy.occView.DisplayShape(Proxy.octShape);
        }

        public void MakeSphere()
        {
            Proxy.occModel.MakeSphere();
            Proxy.Display();
        }

        public void MakeCylinder()
        {
            Proxy.occModel.MakeCylinder();
            Proxy.Display();
        }
    }
}
