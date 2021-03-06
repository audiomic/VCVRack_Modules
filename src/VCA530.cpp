///////////////////////////////////////////////////
//
//   
//
//
///////////////////////////////////////////////////
 
#include "dBiz.hpp"
#include "dsp/digital.hpp"
 
///////////////////////////////////////////////////
struct VCA530 : Module {
    enum ParamIds
    {
        MIX1_PARAM,
        MIX2_PARAM,
        CH1_PARAM,
        CH2_PARAM,
        CH3_PARAM,
        CH4_PARAM,
        CH5_PARAM,
        CH6_PARAM,
        CV1_PARAM,
        CV2_PARAM,
        CV3_PARAM,
        CV4_PARAM,
        CV5_PARAM,
        CV6_PARAM,
        NUM_PARAMS
    };
    enum InputIds
    {
        CH1_INPUT,
        CH1_CV_INPUT,
        CH2_INPUT,
        CH2_CV_INPUT,
        CH3_INPUT,
        CH3_CV_INPUT,
        CH4_INPUT,
        CH4_CV_INPUT,
        CH5_INPUT,
        CH5_CV_INPUT,
        CH6_INPUT,
        CH6_CV_INPUT,
        NUM_INPUTS
    };

    enum OutputIds
    {
        SUM_OUTPUT_R,
        SUM_OUTPUT_L,
        MIX_OUTPUT_R,
        MIX_OUTPUT_L,
        NUM_OUTPUTS
    };

    enum LightIds
    {
        MIX1_LIGHTS,
        CLIP1_LIGHTS,
        MIX2_LIGHTS,
        CLIP2_LIGHTS,
        NUM_LIGHTS
    };

    VCA530() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override ;
};

void VCA530::step()
{

    float cv1 = (clampf(inputs[CH1_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV1_PARAM].value)+1;
    float cv2 = (clampf(inputs[CH2_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV2_PARAM].value)+1;
    float cv3 = (clampf(inputs[CH3_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV3_PARAM].value)+1;
    float cv4 = (clampf(inputs[CH4_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV4_PARAM].value)+1;
    float cv5 = (clampf(inputs[CH5_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV5_PARAM].value)+1;
    float cv6 = (clampf(inputs[CH6_CV_INPUT].normalize(10.0) / 10.0, 0.0, 1.0)) * (params[CV6_PARAM].value)+1;

    float ch1 = inputs[CH1_INPUT].value * params[CH1_PARAM].value * cv1 * cv2 * cv3;
    float ch2 = inputs[CH2_INPUT].value * params[CH2_PARAM].value * cv1 * cv2 * cv3;
    float ch3 = inputs[CH3_INPUT].value * params[CH3_PARAM].value * cv1 * cv2 * cv3;
    float ch4 = inputs[CH4_INPUT].value * params[CH4_PARAM].value * cv4 * cv5 * cv6;
    float ch5 = inputs[CH5_INPUT].value * params[CH5_PARAM].value * cv4 * cv5 * cv6;
    float ch6 = inputs[CH6_INPUT].value * params[CH6_PARAM].value * cv4 * cv5 * cv6;

    float sum_l = (ch1 + ch2 + ch3 + ch4 + ch5 + ch6) * params[MIX1_PARAM].value * cv1 * cv2 * cv3 * cv4 * cv5 * cv6;
    float sum_r = (ch1 + ch2 + ch3 + ch4 + ch5 + ch6) * params[MIX2_PARAM].value * cv1 * cv2 * cv3 * cv4 * cv5 * cv6;

    float mix_l = (ch1 + ch2 + ch3) * params[MIX1_PARAM].value * cv1 * cv2 * cv3;
    float mix_r = (ch4 + ch5 + ch6) * params[MIX2_PARAM].value * cv4 * cv5 * cv6;

    outputs[SUM_OUTPUT_R].value = sum_r;
    outputs[SUM_OUTPUT_L].value = sum_l;
    outputs[MIX_OUTPUT_R].value = mix_r;
    outputs[MIX_OUTPUT_L].value = mix_l;

    lights[MIX1_LIGHTS].value = mix_l;
    lights[MIX2_LIGHTS].value = mix_r;
    if (mix_l>5)
        lights[CLIP1_LIGHTS].value =1.0;
    else
        lights[CLIP1_LIGHTS].value = 0.0;

    if (mix_r > 5)
        lights[CLIP2_LIGHTS].value = 1.0;
    else
        lights[CLIP2_LIGHTS].value = 0.0;
};

VCA530Widget::VCA530Widget()
{
    VCA530 *module = new VCA530();
	setModule(module);
	box.size = Vec(15*12, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		//panel->setBackground(SVG::load("plugins/mental/res/Mixer.svg"));
        panel->setBackground(SVG::load(assetPlugin(plugin,"res/VCA530.svg")));
		addChild(panel);
	}
  int column_1 =-15;

  int top_row = 60;
  int column_spacing = 27;
  int low = 350;
  int low_f = 230 ;
  int med = 180;
  int up =-10;
  


  addParam(createParam<DaviesBlu>(Vec(52,  med + up ), module, VCA530::MIX1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<DaviesBlu>(Vec(129, med + up), module, VCA530::MIX2_PARAM, 0.0, 1.0, 0.0));

  // channel strips

  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 1, 30+up), module, VCA530::CH1_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 2, 30+up), module, VCA530::CH2_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 3, 30+up), module, VCA530::CH3_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 4, 30+up), module, VCA530::CH4_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 5, 30+up), module, VCA530::CH5_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 6, 30+up), module, VCA530::CH6_INPUT));

  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 1 ,top_row+up), module, VCA530::CH1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 2, top_row+up), module, VCA530::CH2_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 3, top_row+up), module, VCA530::CH3_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 4, top_row+up), module, VCA530::CH4_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 5, top_row+up), module, VCA530::CH5_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 6, top_row+up), module, VCA530::CH6_PARAM, 0.0, 1.0, 0.0));

  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 1, low+up), module, VCA530::CH1_CV_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 2, low+up), module, VCA530::CH2_CV_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 3, low+up), module, VCA530::CH3_CV_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 4, low+up), module, VCA530::CH4_CV_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 5, low+up), module, VCA530::CH5_CV_INPUT));
  addInput(createInput<PJ301MIPort>(Vec(column_1 + column_spacing * 6, low+up), module, VCA530::CH6_CV_INPUT));

  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 1 ,low_f+up), module, VCA530::CV1_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 2, low_f+up), module, VCA530::CV2_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 3, low_f+up), module, VCA530::CV3_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 4, low_f+up), module, VCA530::CV4_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 5, low_f+up), module, VCA530::CV5_PARAM, 0.0, 1.0, 0.0));
  addParam(createParam<SlidePot2>(Vec(column_1 + column_spacing * 6, low_f+up), module, VCA530::CV6_PARAM, 0.0, 1.0, 0.0));

  //Screw

  addChild(createScrew<ScrewSilver>(Vec(15, 0)));
  addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 0)));
  addChild(createScrew<ScrewSilver>(Vec(15, 365)));
  addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 365)));

  // outputs
  addOutput(createOutput<PJ301MOPort>(Vec(15 , med+up), module, VCA530::MIX_OUTPUT_L));
  addOutput(createOutput<PJ301MOPort>(Vec(95 , med+up), module, VCA530::MIX_OUTPUT_R));
//
  addOutput(createOutput<PJ301MOPort>(Vec(15, med+25+up), module, VCA530::SUM_OUTPUT_L));
  addOutput(createOutput<PJ301MOPort>(Vec(95, med+25+up), module, VCA530::SUM_OUTPUT_R));


// lights

  addChild(createLight<SmallLight<GreenLight>>(Vec(42,med+5+20), module, VCA530::MIX1_LIGHTS));
  addChild(createLight<SmallLight<GreenLight>>(Vec(122,med+5+20), module, VCA530::MIX2_LIGHTS));
  addChild(createLight<SmallLight<RedLight>>(Vec(42,med-10+20), module, VCA530::CLIP1_LIGHTS));
  addChild(createLight<SmallLight<RedLight>>(Vec(122,med-10+20), module, VCA530::CLIP2_LIGHTS));
}
