/**********************************************************************

Audacity: A Digital Audio Editor

EffectScienFilter.h

Norm C
Mitch Golden
Vaughan Johnson (Preview)

***********************************************************************/

#ifndef __AUDACITY_EFFECT_SCIENFILTER__
#define __AUDACITY_EFFECT_SCIENFILTER__

#include <wx/setup.h> // for wxUSE_* macros

#include <wx/panel.h>

#include "Biquad.h"

#include "Effect.h"

class wxBitmap;
class wxChoice;
class wxSlider;
class wxStaticText;
class wxTextCtrl;
class RulerPanel;
class ShuttleGui;

#define CLASSICFILTERS_PLUGIN_SYMBOL ComponentInterfaceSymbol{ XO("Classic Filters") }

class EffectScienFilterPanel;

class EffectScienFilter final : public Effect
{
public:
   EffectScienFilter();
   virtual ~EffectScienFilter();

   // ComponentInterface implementation

   ComponentInterfaceSymbol GetSymbol() override;
   wxString GetDescription() override;
   wxString ManualPage() override;

   // EffectDefinitionInterface implementation

   EffectType GetType() override;

   // EffectClientInterface implementation

   unsigned GetAudioInCount() override;
   unsigned GetAudioOutCount() override;
   bool ProcessInitialize(sampleCount totalLen, ChannelNames chanMap = NULL) override;
   size_t ProcessBlock(float **inBlock, float **outBlock, size_t blockLen) override;
   bool DefineParams( ShuttleParams & S ) override;
   bool GetAutomationParameters(CommandParameters & parms) override;
   bool SetAutomationParameters(CommandParameters & parms) override;

   // Effect implementation

   bool Startup() override;
   bool Init() override;
   void PopulateOrExchange(ShuttleGui & S) override;
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

private:
   // EffectScienFilter implementation

   bool TransferGraphLimitsFromWindow();
   bool CalcFilter();
   double ChebyPoly (int Order, double NormFreq);
   float FilterMagnAtFreq(float Freq);

   bool CalcFilterCoeffs (void);

   void EnableDisableRippleCtl (int FilterType);

   void OnSize( wxSizeEvent & evt );
   void OnSlider( wxCommandEvent & evt );

   void OnOrder( wxCommandEvent & evt );
   void OnCutoff( wxCommandEvent & evt );
   void OnRipple( wxCommandEvent & evt );
   void OnStopbandRipple( wxCommandEvent & evt );
   void OnFilterType( wxCommandEvent & evt );
   void OnFilterSubtype( wxCommandEvent & evt );

   void OnSliderDBMAX( wxCommandEvent & evt );
   void OnSliderDBMIN( wxCommandEvent & evt );

private:
   float mCutoff;
   float mRipple;
   float mStopbandRipple;
   int mFilterType;		// Butterworth etc.
   int mFilterSubtype;	// lowpass, highpass
   int mOrder;
   int mOrderIndex;
   ArrayOf<Biquad> mpBiquad;

   double mdBMax;
   double mdBMin;
   bool mEditingBatchParams;

   double mLoFreq;
   double mNyquist;

   EffectScienFilterPanel *mPanel;
   wxSlider *mdBMinSlider;
   wxSlider *mdBMaxSlider;

   wxStaticText *mRippleCtlP;
   wxTextCtrl *mRippleCtl;
   wxStaticText *mRippleCtlU;

   wxTextCtrl *mCutoffCtl;

   wxStaticText *mStopbandRippleCtlP;
   wxTextCtrl *mStopbandRippleCtl;
   wxStaticText *mStopbandRippleCtlU;

   wxChoice *mFilterTypeCtl;
   wxChoice *mFilterSubTypeCtl;
   wxChoice *mFilterOrderCtl;

   RulerPanel *mdBRuler;
   RulerPanel *mfreqRuler;

   DECLARE_EVENT_TABLE()

   friend class EffectScienFilterPanel;
};

class EffectScienFilterPanel final : public wxPanelWrapper
{
public:
   EffectScienFilterPanel(
      wxWindow *parent, wxWindowID winid,
      EffectScienFilter *effect, double lo, double hi);
   virtual ~EffectScienFilterPanel();

   // We don't need or want to accept focus.
   bool AcceptsFocus() const;
   // So that wxPanel is not included in Tab traversal - see wxWidgets bug 15581
   bool AcceptsFocusFromKeyboard() const;

   void SetFreqRange(double lo, double hi);
   void SetDbRange(double min, double max);

private:
   void OnPaint(wxPaintEvent & evt);
   void OnSize(wxSizeEvent & evt);

private:
   EffectScienFilter *mEffect;
   wxWindow *mParent;

   double mLoFreq;
   double mHiFreq;

   double mDbMin;
   double mDbMax;

   std::unique_ptr<wxBitmap> mBitmap;
   wxRect mEnvRect;
   int mWidth;
   int mHeight;

   friend class EffectScienFilter;

   DECLARE_EVENT_TABLE()
};

#if wxUSE_ACCESSIBILITY

// ScienceFilter and Equalisation effects both need SliderAx class.
// For now it is declared and defined in Equalisation effect.
// TODO: Move it to its own file.

#endif // wxUSE_ACCESSIBILITY

#endif
