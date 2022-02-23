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
        public OCCT.NativeWrapper Wrapper { get; private set; }

        public OCCProxyer()
        {
            Proxy = new OCCProxy();
            Wrapper = new OCCT.NativeWrapper();
        }

        public void MakeBox()
        {
            Proxy.occModel.MakeBox();
            Proxy.Display();
        }

        public void MakeCone()
        {
            Proxy.MakeOctShape();
            OCCT.OCCT_Shape octShp = Proxy.GetOctShape();
            Proxy.occView.DisplayShape(octShp);
        }

        public void MakeTorus()
        {
            Wrapper.makeTorus();
            OCCT.OCCT_Shape octShp = Wrapper.getShape();
            Proxy.occView.DisplayShape(octShp);
        }

        public void MakeWedge()
        {
            Wrapper.makeWedge();
            OCCT.OCCT_Shape octShp = Wrapper.getShape();
            Proxy.occView.DisplayShape(octShp);
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
