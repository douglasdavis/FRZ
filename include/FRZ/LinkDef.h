#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class FRZ::Lepton+;
#pragma link C++ class FRZ::MET+;
#pragma link C++ class FRZ::LeptonPair+;
#pragma link C++ class FRZ::Jet+;
#pragma link C++ class FRZ::PhysicsObject<FRZ::Lepton>+;
#pragma link C++ class FRZ::PhysicsObject<FRZ::MET>+;
#pragma link C++ class FRZ::PhysicsObject<FRZ::LeptonPair>+;
#pragma link C++ class FRZ::PhysicsObject<FRZ::Jet>+;
#pragma link C++ class std::vector<FRZ::PhysicsObject<FRZ::Lepton> >+;
#pragma link C++ class std::vector<FRZ::PhysicsObject<FRZ::MET> >+;
#pragma link C++ class std::vector<FRZ::PhysicsObject<FRZ::LeptonPair> >+;
#pragma link C++ class std::vector<FRZ::PhysicsObject<FRZ::Jet> >+;
#pragma link C++ class FRZ::FinalState+;
#pragma link C++ class FRZ::Sample+;
#pragma link C++ class FRZ::SampleHolder+;
#pragma link C++ class FRZ::HistMaker+;

#endif
