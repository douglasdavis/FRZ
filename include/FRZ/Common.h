/** @file  Common.h
 *  @brief FRZ::Common header file
 *
 *  This file contains key definitions
 *
 *  @author Douglas Davis < ddavis@phy.duke.edu >
 */

#ifndef FRZ_COMMON_H
#define FRZ_COMMON_H

#define LUMI           20276.9       // unscaled luminosity
#define LUM_DATA       LUMI          // chosen data luminosity
#define ELECTRON_MASS  0.000511      // electron mass [GeV]
#define MUON_MASS      0.105658      // muon mass [GeV]
#define PERIOD_L_START 187816        // first run number of period L
#define GeV            1000.0        // 1 GeV in MeV (crunching numbers usually MeV)
#define MeV            0.0010        // 1 MeV in GeV (plotting usually GeV)
#define BTAG_MV1_60    0.9827        // MV1 60% working point
#define BTAG_MV1_70    0.7892        // MV1 70% working point
#define BTAG_MV1_80    0.3511        // MV1 80% working point
#define BTAG_CUTOFF    BTAG_MV1_70   // chosen b-tagging working point
#define LEPSIZE        7             // maximal number of leptons
#define JETSIZE        30            // maximal number of jets

#define Z_MINUS_15     76187.6       // Z mass - 15 GeV (in MeV)
#define Z_PLUS_15      106187.6      // Z mass + 15 GeV (in MeV)
#define Z_MINUS_10     81187.6       // Z mass - 10 GeV (in MeV)
#define Z_PLUS_10      101187.6      // Z mass + 10 GeV (in MeV)

#define GOOD_ELEC      0b11110000011011111
#define LOOSE_ELEC     0b00010000001011111

#define GOOD_MUON      0b10001111101111
#define LOOSE_MUON     0b00001111101111

#endif
