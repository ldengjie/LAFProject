#include "LafKernel/PhyEvent/MuonRecSimple.h"

MuonRecSimple::MuonRecSimple()
    : DataSec(),
      DetectorId(0),
      PointX(0),
      PointY(0),
      PointZ(0)
{
}

MuonRecSimple::MuonRecSimple(const MuonRecSimple& other)
    : DataSec(),
      EndIndex(other.EndIndex),
      FiredDet(other.FiredDet),
      Flag(other.Flag),
      IWSNhit(other.IWSNhit),
      OWSNhit(other.OWSNhit),
      StartIndex(other.StartIndex)
{
   DetectorId = new std::vector<int>(*other.DetectorId);
   PointX = new std::vector<float>(*other.PointX);
   PointY = new std::vector<float>(*other.PointY);
   PointZ = new std::vector<float>(*other.PointZ);
}

MuonRecSimple::~MuonRecSimple()
{
   delete DetectorId;
   delete PointX;
   delete PointY;
   delete PointZ;
}
