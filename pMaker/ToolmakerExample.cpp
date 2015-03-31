
office_doctor.com

the overwhelming evidence is ... we behave like animals




class MyWriteAction : public SoWriteAction {
    SO_ACTION_HEADER(SoWriteAction);

  public:
    MyWriteAction(SoOutput * out);
    virtual ~MyWriteAction();

    static void initClass(void);

  protected:
    virtual void beginTraversal(SoNode * node);

  private:
    static void actionMethod(SoAction *, SoNode *);
    int nrnodes;
    int totalnrnodes;
  };


  SO_ACTION_SOURCE(MyWriteAction);

  MyWriteAction::MyWriteAction(SoOutput * out)
    : SoWriteAction(out)
  {
    SO_ACTION_CONSTRUCTOR(MyWriteAction);
  }

  MyWriteAction::~MyWriteAction()
  {
  }

  void
  MyWriteAction::initClass(void)
  {
    SO_ACTION_INIT_CLASS(MyWriteAction, SoWriteAction);

    SO_ACTION_ADD_METHOD(SoNode, MyWriteAction::actionMethod);
  }

  void
  MyWriteAction::beginTraversal(SoNode * node)
  {
    this->nrnodes = 0;
    this->totalnrnodes = 0;
    SoWriteAction::beginTraversal(node);
  }

  void
  MyWriteAction::actionMethod(SoAction * a, SoNode * n)
  {
    // To abort the export process in mid-writing, we could just avoid
    // calling in to the SoNode::writeS() method.
    SoNode::writeS(a, n);

    MyWriteAction * mwa = (MyWriteAction *)a;
    SoOutput * out = mwa->getOutput();
    if (out->getStage() == SoOutput::COUNT_REFS) {
      mwa->totalnrnodes++;
    }
    else { //  (out->getStage() == SoOutput::WRITE)
      mwa->nrnodes++;
      SbString s;
      s.sprintf(" # wrote node %p (%d/%d) \n", n, mwa->nrnodes, mwa->totalnrnodes);
      out->write(s.getString());
    }
  }
  
  
  08/30/12  				4 Mclendon Hardware Wh Seattle Wa 					  				$26.21  		
		08/29/12  				Usps 54764700435101393 Seattle Wa 					  				$15.54  		
		08/27/12  				Perine Danforth Co Llc 209-830-2860 Wa 					  				$99.95  		
		08/08/12  				Internet Payment Thank You 					$22.93  				  		
		08/03/12  				The Home Depot #8944 Seattle Wa 					  				$22.93  		
		07/19/12  				Internet Payment Thank You 					$10.48  				  		
		07/16/12  				Interest Charge On Purchases 					  				$10.48  		
		07/16/12  				Internet Payment Thank You 					$457.37  				  		
		07/10/12  				Late Fee - Payment Due On 07/10 					  				$39.00  		
		06/11/12  				Central Welding Supply Seattle Wa 					  				$58.52  		
		06/08/12  				Central Welding Supply Seattle Wa 					  				$23.21  		
		06/04/12  				Central Welding Supply Seattle Wa 					  				$204.12  		
		06/04/12  				Ken's Market #75-11 Seattle Wa 					  				$10.72  		
		06/01/12  				Lucky 5 Seattle Wa 					  				$16.80  		
		06/01/12  				Lucky 5 Seattle Wa 					  				$75.00  		
		06/01/12  				Spu So Recyc/cty #3102 Seattle Wa 					  				$30.00  		
		04/18/12  				Internet Payment Thank You 					$30.26  				  		
		04/16/12  				Beveridge Place Pub Seattle Wa 					  				$5.50  		
		04/13/12  				Annual Membership Fee 					  				$0.00  		
		04/13/12  				Kokoras Greek Grill Seattle Wa 					  				$24.76  		
		04/09/12  				Internet Payment Thank You 					$13.81  				  		
		03/15/12  				Interest Charge On Purchases 					  				$13.81  		
		03/15/12  				Internet Payment Thank You 					$634.55  				  		
		03/12/12  				Late Fee - Payment Due On 03/10 					  				$39.00  		
		02/14/12  				Overlimit Fee 					  				$39.00  		
		02/14/12  				Interest Charge On Purchases 					  				$12.97  		
		02/10/12  				Late Fee - Payment Due On 02/10 					  				$39.00  		
		01/17/12  				Staples 00113613 Seattle Wa 					  				$16.90  		
		01/13/12  				Interest Charge On Purchases 					  				$9.77  		
		01/10/12  				Late Fee - Payment Due On 01/10 					  				$39.00