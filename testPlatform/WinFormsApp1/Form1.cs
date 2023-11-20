using WinFormsApp1.Models;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        public static Dictionary<string, Lighting> lightingPreset;
        public static Dictionary<string, Lighting> getLightingPresets()
        {
            var rtrn = new Dictionary<string, Lighting>();
            var alternating = new Lighting(0);
            alternating.append_colorarray(Colors.AsColor(0xFF, 0x00, 0x00, 0x00));
            alternating.append_colorarray(Colors.AsColor(0x00, 0xFF, 0x00, 0x00));
            alternating.append_colorarray(Colors.AsColor(0x00, 0x00, 0xFF, 0x00));
            alternating.append_colorarray(Colors.AsColor(0x00, 0x00, 0x00, 0xFF));
            alternating.set_colormode("ORDERED");
            alternating.set_brightness(0xFF);
            alternating.set_transitionmode("BLEND");
            alternating.set_transitionshiftmode("ORDERED");
            alternating.set_transitionshifttarget("DOORS");
            alternating.set_transitionshiftamount(127);
            alternating.set_transitionspeed(0xFD, 0);
            rtrn.Add("alternating", alternating);
            var fadeBounce = new Lighting(0);
            fadeBounce.append_colorarray(Colors.AsColor(0xFF, 0x00, 0x00, 0x00));
            fadeBounce.append_colorarray(Colors.AsColor(0x00, 0xFF, 0x00, 0x00));
            fadeBounce.append_colorarray(Colors.AsColor(0x00, 0x00, 0xFF, 0x00));
            fadeBounce.append_colorarray(Colors.AsColor(0x00, 0x00, 0x00, 0xFF));
            fadeBounce.set_colormode("ORDERED");
            fadeBounce.set_brightness(0xFF);
            fadeBounce.set_transitionmode("ONBOUNCE");
            fadeBounce.set_fademode("PULSE");
            fadeBounce.set_fadespeed(0xFD, 0);
            rtrn.Add("fadeBounce", fadeBounce);
            var hearbeat = new Lighting(0);
            hearbeat.set_color(Colors.AsColor(0xFF, 0x00, 0x00, 0x00));
            hearbeat.set_colormode("CONSTANT");
            hearbeat.set_brightness(0xFF);
            hearbeat.set_fademode("HEARTBEAT");
            hearbeat.set_fadespeed(0xFD, 0);
            rtrn.Add("hearbeat", hearbeat);
            var chaseTest = new Lighting(0);
            chaseTest.append_colorarray(Colors.AsColor(0xFF, 0x00, 0x00, 0x00));
            chaseTest.append_colorarray(Colors.AsColor(0x00, 0xFF, 0x00, 0x00));
            chaseTest.append_colorarray(Colors.AsColor(0x00, 0x00, 0xFF, 0x00));
            chaseTest.append_colorarray(Colors.AsColor(0x00, 0x00, 0x00, 0xFF));
            chaseTest.set_colormode("ORDERED");
            chaseTest.set_brightness(0xFF);
            chaseTest.set_transitionspeed(0xFF, 0);
            chaseTest.set_transitionmode("CHASE");
            chaseTest.set_transitionchasemode("BOUNCE");
            chaseTest.set_transitionchasewidth(127);
            chaseTest.set_transitionwidth(127);
            // chaseTest.set_transitionshifttarget("DOORS");
            //chaseTest.set_transitionshiftamount(1);
            //chaseTest.set_transitionspeed(0xFD, 0);
            rtrn.Add("chase test", chaseTest);
            return rtrn;
        }
        public static Dictionary<string, ulong[]> presetColorArrays;
        public static Dictionary<string, ulong[]> getPresetColorArrays()
        {
            Dictionary<string, ulong[]> rtrn = new Dictionary<string, ulong[]>();
            rtrn.Add("All Colors", new ulong[4]{
                Colors.AsColor(0xFF, 0x00, 0x00, 0x00),
                Colors.AsColor(0x00, 0xFF, 0x00, 0x00),
                Colors.AsColor(0x00, 0x00, 0xFF, 0x00),
                Colors.AsColor(0x00, 0x00, 0x00, 0xFF)
            });
            rtrn.Add("Two Colors", new ulong[2]{
                Colors.AsColor(0xFF, 0x00, 0x00, 0x00),
                Colors.AsColor(0x00, 0x00, 0xFF, 0x00)
            });
            return rtrn;
        }
        public static Dictionary<string, ulong> presetBgColors;
        public static Dictionary<string, ulong> getPresetBgColors()
        {
            Dictionary<string, ulong> rtrn = new Dictionary<string, ulong>();
            rtrn.Add("Black", Colors.AsColor(0x00, 0x00, 0x00, 0x00));
            rtrn.Add("White", Colors.AsColor(0x00, 0x00, 0x00, 0xFF));
            rtrn.Add("Red", Colors.AsColor(0xFF, 0x00, 0x00, 0x00));
            return rtrn;
        }
        public Form1()
        {
            InitializeComponent();
            lightingPreset = getLightingPresets();
            presetColorArrays = getPresetColorArrays();
            presetBgColors = getPresetBgColors();

            helpProvider1.SetShowHelp(nmPixel, true);
            helpProvider1.SetHelpString(nmPixel, "Select a pixel to see the corresponding effects from at this point.");
            helpProvider1.SetShowHelp(nmTicks, true);
            helpProvider1.SetHelpString(nmTicks, "Increments the drawing. Corresponds to milliseconds, but real time is actually much slower than ms based on redraws.");

            helpProvider1.SetShowHelp(nmFadeChaseWidth, true);
            helpProvider1.SetShowHelp(nmFadeFlicker, true);
            helpProvider1.SetShowHelp(nmFadeShiftAmount, true);
            helpProvider1.SetShowHelp(nmFadeShiftWidth, true);
            helpProvider1.SetShowHelp(nmFadeSpeed, true);
            helpProvider1.SetShowHelp(nmTransitionChaseWidth, true);
            helpProvider1.SetShowHelp(nmTransitionFlicker, true);
            helpProvider1.SetShowHelp(nmTransitionShiftAmount, true);
            helpProvider1.SetShowHelp(nmTransitionShiftWidth, true);
            helpProvider1.SetShowHelp(nmTransitionWidth, true);
        }

        private void redraw()
        {
            #region Refresh lighting display
            var lighting = GetSelectedLighting();
            Draw.RenderLights((ulong)nmTicks.Value, lighting, (Bitmap)picDisplay.Image);
            picDisplay.Refresh();
            #endregion

            #region Refresh lighting values
            var pixel = (ushort)nmPixel.Value;

            var fCycle = lighting._fade.get_cycle_percent((ulong)nmTicks.Value);
            var fChaseEffect = lighting._fade._chase.get_effect(fCycle, lighting._fade.bounced, pixel);
            var fShiftEffect = lighting._fade._shift.get_effect(pixel, (pixel == 0 && fCycle < 0.5 && !lighting._fade._bottomhalf), 2);

            var tCycle = lighting._transition.get_cycle_percent((ulong)nmTicks.Value);
            var tChaseEffect = lighting._transition._chase.get_effect(tCycle, lighting._fade.bounced, pixel);
            bool shiftBounced = (pixel == 0 && lighting._fade.bounced || lighting._transition._lastP0 > 0 && (int)(tCycle * lighting._transition._colorarray_length) == 0);
            var tShiftEffect = lighting._transition._shift.get_effect(pixel, shiftBounced, lighting._transition._colorarray_length);

            var cyclePercent = tCycle + tShiftEffect;
            cyclePercent = cyclePercent - ((int)cyclePercent);

            int colorBucket = (int)(cyclePercent * lighting._transition._colorarray_length);

            txtColorBucket.Text = colorBucket.ToString();
            prgTransitionCycle.Value = (int)(100 * (tCycle - Math.Truncate(tCycle)));
            txtTransitionChaseEffect.Text = tChaseEffect.ToString();
            txtTransitionShiftEffect.Text = tShiftEffect.ToString();
            nmTransitionShiftStep.Value = lighting._transition._shift._step;

            prgFadeCycle.Value = (int)(100 * fCycle);
            txtFadeChaseEffect.Text = fChaseEffect.ToString();
            txtFadeShiftEffect.Text = fShiftEffect.ToString();
            nmFadeShiftStep.Value = lighting._fade._shift._step;
            #endregion

            #region Enable / disable options and set help
            if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_OFF)
            {
                nmTransitionSpeed.Enabled = false;
                nmTransitionChaseWidth.Enabled = false;
                nmTransitionFlicker.Enabled = false;
                nmTransitionWidth.Enabled = false;
                grpTransitionChase.Enabled = false;
                grpTransitionShift.Enabled = false;
            }
            else if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_IMMIDIATE || lighting._transition._mode == TransitionMode.TRANSITIONMODE_BLEND || lighting._transition._mode == TransitionMode.TRANSITIONMODE_ONBOUNCE)
            {
                nmTransitionSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionSpeed, "Time between cycles");
                nmTransitionChaseWidth.Enabled = false;
                nmTransitionFlicker.Enabled = false;
                nmTransitionWidth.Enabled = false;
                grpTransitionChase.Enabled = false;
                grpTransitionShift.Enabled = true;
            }
            else if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_STICKYBLEND)
            {
                nmTransitionSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionSpeed, "Time between cycles");
                nmTransitionFlicker.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionWidth, "Length of time full off");
                nmTransitionWidth.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionWidth, "Length of time full on");
                grpTransitionChase.Enabled = false;
                grpTransitionShift.Enabled = true;
            }
            else if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_CHASE)
            {
                nmTransitionSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionSpeed, "Time between cycles");
                nmTransitionFlicker.Enabled = false;
                nmTransitionWidth.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionWidth, "Width of effected cycle, as a percent of full cycle (compression)");
                grpTransitionChase.Enabled = true;
                grpTransitionShift.Enabled = true;

                nmTransitionChaseWidth.Enabled = lighting._transition._chase._mode != ChaseMode.CHASEMODE_OFF;
                helpProvider1.SetHelpString(nmTransitionChaseWidth, "Number of cycles \"in Range\" at one time.");
                cmbTransitionChaseTarget.Enabled = lighting._transition._chase._mode != ChaseMode.CHASEMODE_OFF;
            }
            else if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_FLICKER)
            {
                nmTransitionSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionSpeed, "Time on for \"flicker\"");
                nmTransitionFlicker.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionFlicker, "Number of pixels turning on/ off");
                nmTransitionWidth.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionWidth, "Maximum size of activated \"flicker\" pixels (distance from center to impact)");
                grpTransitionChase.Enabled = false;
                grpTransitionShift.Enabled = true;
            }
            else if (lighting._transition._mode == TransitionMode.TRANSITIONMODE_PIXELATE)
            {
                nmTransitionSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionSpeed, "Time between cycles");
                nmTransitionFlicker.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionFlicker, "Max number of pixels to switch on at once");
                nmTransitionWidth.Enabled = true;
                helpProvider1.SetHelpString(nmTransitionWidth, "Percent of time fully on");
                grpTransitionChase.Enabled = false;
                grpTransitionShift.Enabled = true;
            }
            if (grpTransitionShift.Enabled)
            {
                if (lighting._transition._shift._mode == ShiftMode.SHIFTMODE_OFF)
                {
                    nmTransitionShiftAmount.Enabled = false;
                    nmTransitionShiftWidth.Enabled = false;
                }
                else
                {
                    nmTransitionShiftAmount.Enabled = true;
                    if (lighting._transition._shift._target == ShiftTarget.SHIFTTARGET_PIXEL)
                    { nmTransitionShiftWidth.Enabled = true; }
                    else
                    { nmTransitionShiftWidth.Enabled = false; }
                }
            }

            if (lighting._fade._mode == FadeMode.FADEMODE_OFF)
            {
                nmFadeSpeed.Enabled = false;
                nmFadeWidth.Enabled = false;
                nmFadeFlicker.Enabled = false;
                grpFadeChase.Enabled = false;
                grpFadeShift.Enabled = false;
            }
            else if (lighting._fade._mode == FadeMode.FADEMODE_PULSE || lighting._fade._mode == FadeMode.FADEMODE_LINEAR)
            {
                nmFadeSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmFadeSpeed, "Time between pulses");
                nmFadeWidth.Enabled = true;
                helpProvider1.SetHelpString(nmFadeWidth, "Length of time on");
                nmFadeFlicker.Enabled = true;
                helpProvider1.SetHelpString(nmFadeFlicker, "Length of time off");
                grpFadeChase.Enabled = false;
                grpFadeShift.Enabled = true;
            }
            else if (lighting._fade._mode == FadeMode.FADEMODE_HEARTBEAT)
            {
                nmFadeSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmFadeSpeed, "Time between beats");
                nmFadeWidth.Enabled = true;
                helpProvider1.SetHelpString(nmFadeWidth, "amount of cycle on vs off");
                nmFadeFlicker.Enabled = false; // maybe in the future?
                //helpProvider1.SetHelpString(nmFadeFlicker, "Size of small beat to large?");
                grpFadeChase.Enabled = false;
                grpFadeShift.Enabled = true;
            }
            else if (lighting._fade._mode == FadeMode.FADEMODE_CHASE)
            {
                nmFadeSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmFadeSpeed, "Time between chase cycles");
                nmFadeWidth.Enabled = true;
                helpProvider1.SetHelpString(nmFadeWidth, "Width of effected cycle, as a percent (compresses waves)");
                nmFadeFlicker.Enabled = false;
                grpFadeChase.Enabled = true;
                grpFadeShift.Enabled = true;

                nmFadeChaseWidth.Enabled = lighting._fade._chase._mode != ChaseMode.CHASEMODE_OFF;
                helpProvider1.SetHelpString(nmFadeChaseWidth, "Number of cycles \"in Range\" at one time.");
            }
            else if (lighting._fade._mode == FadeMode.FADEMODE_FLICKER)
            {
                nmFadeSpeed.Enabled = true;
                helpProvider1.SetHelpString(nmFadeSpeed, "Time on for \"flicker\"");
                nmFadeWidth.Enabled = true;
                helpProvider1.SetHelpString(nmFadeWidth, "Maximum size of activated \"flicker\" pixels (distance from center to impact)");
                nmFadeFlicker.Enabled = true;
                helpProvider1.SetHelpString(nmFadeFlicker, "Number of pixels turning on/ off");
                grpFadeChase.Enabled = false;
                grpFadeShift.Enabled = true;
            }
            if (grpFadeShift.Enabled)
            {
                if (lighting._fade._shift._mode == ShiftMode.SHIFTMODE_OFF)
                {
                    nmFadeShiftAmount.Enabled = false;
                    nmFadeShiftWidth.Enabled = false;
                }
                else
                {
                    nmFadeShiftAmount.Enabled = true;
                    if (lighting._fade._shift._target == ShiftTarget.SHIFTTARGET_PIXEL)
                    { nmFadeShiftWidth.Enabled = true; }
                    else
                    { nmFadeShiftWidth.Enabled = false; }
                }
            }
            #endregion
        }
        private void nmTicks_ValueChanged(object sender, EventArgs e)
        {
            //Graphics g = Graphics.FromImage(bmp);
            //g.Clear(Color.Green);

            redraw();
            // Increment And Draw
            // Draw on picDisplay

            //((Bitmap)pictureBox1.Image).SetPixel(currentX, currentY, Color.FromArgb(r, g, b));
            //pictureBox1.Refresh();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            /*for (var i = 0; i < 100; i++)
            {
                var cyclePercent = i / 100.0;
                var effect = 1 - ((cyclePercent < 0.25 ? LightingSupport.unitSineWave(4 * cyclePercent) : 0) + (cyclePercent > 0.15 && cyclePercent < 0.48333 ? 0.8 * LightingSupport.unitSineWave(3 * (cyclePercent - 0.15)) : 0));
                //var effect = (cyclePercent < 0.25 ? LightingSupport.unitSineWave(4 * cyclePercent) : 0) + (cyclePercent > 0.15 && cyclePercent < 0.48333 ? 0.8 * LightingSupport.unitSineWave(3 * (cyclePercent - 0.15)) : 0);
                System.Diagnostics.Debug.WriteLine($"{i}: {effect}");
            }*/
            picDisplay.Image = new Bitmap(802, 426);
            
            // Build defaults in drop downs
            // cmbTransitionMode
            cmbTransitionMode.Items.Add("OFF");
            cmbTransitionMode.Items.Add("IMMIDIATE");
            cmbTransitionMode.Items.Add("BLEND");
            cmbTransitionMode.Items.Add("STICKYBLEND");
            cmbTransitionMode.Items.Add("ONBOUNCE");
            cmbTransitionMode.Items.Add("CHASE");
            cmbTransitionMode.Items.Add("FLICKER");
            cmbTransitionMode.Items.Add("PIXELATE");
            // cmbColorArray
            cmbColorArray.DataSource = null;
            cmbColorArray.ValueMember = "Value";
            cmbColorArray.DisplayMember = "Key";
            cmbColorArray.DataSource = new BindingSource(presetColorArrays, null);
            // cmbTransitionChaseMode
            cmbTransitionChaseMode.Items.Add("OFF");
            cmbTransitionChaseMode.Items.Add("BOUNCE");
            cmbTransitionChaseMode.Items.Add("OUTWARD");
            cmbTransitionChaseMode.Items.Add("INWARD");
            cmbTransitionChaseMode.Items.Add("CENTERBOUNCE");
            cmbTransitionChaseMode.Items.Add("CLOCKWISE");
            cmbTransitionChaseMode.Items.Add("COUNTER");
            // cmbTransitionChaseTarget
            cmbTransitionChaseTarget.Items.Add("ALL");
            cmbTransitionChaseTarget.Items.Add("DOORS");
            cmbTransitionChaseTarget.Items.Add("CABINETS");
            cmbTransitionChaseTarget.Items.Add("SIDES");
            // cmbTransitionShiftMode
            cmbTransitionShiftMode.Items.Add("OFF");
            cmbTransitionShiftMode.Items.Add("ORDERED");
            cmbTransitionShiftMode.Items.Add("ALTERNATE");
            cmbTransitionShiftMode.Items.Add("RANDOM");
            // cmbTransitionShiftTarget
            cmbTransitionShiftTarget.Items.Add("PIXEL");
            cmbTransitionShiftTarget.Items.Add("DOORS");
            cmbTransitionShiftTarget.Items.Add("CABINETS");
            cmbTransitionShiftTarget.Items.Add("SIDES");
            // cmbFadeMode
            cmbFadeMode.Items.Add("OFF");
            cmbFadeMode.Items.Add("PULSE");
            cmbFadeMode.Items.Add("LINEAR");
            cmbFadeMode.Items.Add("HEARTBEAT");
            cmbFadeMode.Items.Add("CHASE");
            cmbFadeMode.Items.Add("FLICKER");
            // cmbFadeBgColor
            cmbFadeBgColor.DataSource = new BindingSource(presetBgColors, null);
            cmbFadeBgColor.DisplayMember = "Key";
            cmbFadeBgColor.ValueMember = "Value";
            // cmbFadeChaseMode
            cmbFadeChaseMode.Items.Add("OFF");
            cmbFadeChaseMode.Items.Add("BOUNCE");
            cmbFadeChaseMode.Items.Add("OUTWARD");
            cmbFadeChaseMode.Items.Add("INWARD");
            cmbFadeChaseMode.Items.Add("CENTERBOUNCE");
            cmbFadeChaseMode.Items.Add("CLOCKWISE");
            cmbFadeChaseMode.Items.Add("COUNTER");
            // cmbFadeChaseTarget
            cmbFadeChaseTarget.Items.Add("ALL");
            cmbFadeChaseTarget.Items.Add("DOORS");
            cmbFadeChaseTarget.Items.Add("CABINETS");
            cmbFadeChaseTarget.Items.Add("SIDES");
            // cmbFadeShiftMode
            cmbFadeShiftMode.Items.Add("OFF");
            cmbFadeShiftMode.Items.Add("ORDERED");
            cmbFadeShiftMode.Items.Add("ALTERNATE");
            cmbFadeShiftMode.Items.Add("RANDOM");
            // cmbFadeShiftTarget
            cmbFadeShiftTarget.Items.Add("PIXEL");
            cmbFadeShiftTarget.Items.Add("DOORS");
            cmbFadeShiftTarget.Items.Add("CABINETS");
            cmbFadeShiftTarget.Items.Add("SIDES");

            // cmbPresets
            cmbPresets.DataSource = new BindingSource(lightingPreset, null);
            cmbPresets.DisplayMember = "Key";
            cmbPresets.ValueMember = "Value";

            // Trigger draw
            redraw();
        }
        private KeyValuePair<string, ulong[]> GetColorArrayForSelect()
        {
            var lighting = GetSelectedLighting();
            var currentArray = lighting._transition._colorarray.Take(lighting._transition._colorarray_length);
            return presetColorArrays.FirstOrDefault(x => Enumerable.SequenceEqual(x.Value, currentArray));
        }
        private KeyValuePair<string, ulong> GetBgColorForSelect()
        {
            var lighting = GetSelectedLighting();
            return presetBgColors.FirstOrDefault(x => x.Value == lighting._fade._bgcolor);
        }
        private void cmbPresets_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            cmbTransitionMode.SelectedItem = LightingTransition.transitionmode2string(lighting._transition._mode);

            var colorArray = GetColorArrayForSelect();
            cmbColorArray.SelectedItem = (colorArray.Value == null) ? null : colorArray;
            cmbTransitionChaseMode.SelectedItem = LightingChase.chasemode2string(lighting._transition._chase._mode);
            cmbTransitionChaseTarget.SelectedItem = LightingChase.chasetarget2string(lighting._transition._chase._target);
            cmbTransitionShiftMode.SelectedItem = LightingShift.shiftmode2string(lighting._transition._shift._mode);
            cmbTransitionShiftTarget.SelectedItem = LightingShift.shifttarget2string(lighting._transition._shift._target);
            cmbFadeMode.SelectedItem = LightingFade.fademode2string(lighting._fade._mode);

            var bgColor = GetBgColorForSelect();
            cmbFadeBgColor.SelectedItem = (colorArray.Value == null) ? null : bgColor;
            cmbFadeChaseMode.SelectedItem = LightingChase.chasemode2string(lighting._fade._chase._mode);
            cmbFadeChaseTarget.SelectedItem = LightingChase.chasetarget2string(lighting._fade._chase._target);
            cmbFadeShiftMode.SelectedItem = LightingShift.shiftmode2string(lighting._fade._shift._mode);
            cmbFadeShiftTarget.SelectedItem = LightingShift.shifttarget2string(lighting._fade._shift._target);

            nmTransitionWidth.Value = lighting._transition._width;
            nmTransitionSpeed.Value = lighting._transition._speed;
            nmTransitionFlicker.Value = lighting._transition._flickeractivation;
            nmTransitionChaseWidth.Value = lighting._transition._chase._width;
            nmTransitionShiftAmount.Value = lighting._transition._shift._amount;

            nmFadeSpeed.Value = lighting._fade._speed;
            nmFadeFlicker.Value = lighting._fade._flickeractivation;
            nmFadeChaseWidth.Value = lighting._fade._chase._width;
            nmFadeShiftAmount.Value = lighting._fade._shift._amount;

            redraw();
        }

        private void nmPixel_ValueChanged(object sender, EventArgs e)
        {
            // Change output display for transition and fade...
            redraw();
        }

        private void cmbTimeStep_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Change step of nmPixel...
            nmTicks.Increment = int.Parse((string)cmbTimeStep.SelectedItem);
        }

        private void rdoManual_CheckedChanged(object sender, EventArgs e)
        {
            if (rdoManual.Checked)
            {
                rdoSlow.Checked = false;
                rdoRealTime.Checked = false;
                timer1.Stop();
            }
        }

        private void rdoSlow_CheckedChanged(object sender, EventArgs e)
        {
            if (rdoSlow.Checked)
            {
                rdoManual.Checked = false;
                rdoRealTime.Checked = false;
                timer1.Interval = 500;
                timer1.Start();
            }
        }

        private void rdoRealTime_CheckedChanged(object sender, EventArgs e)
        {
            if (rdoRealTime.Checked)
            {
                rdoSlow.Checked = false;
                rdoManual.Checked = false;
                timer1.Interval = 1;
                timer1.Start();
            }
        }

        private void cmbTransitionMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            // change transition mode
            var lighting = ((KeyValuePair<string, Lighting>)cmbPresets.SelectedItem).Value;
            if ((string)cmbTransitionMode.SelectedItem != LightingTransition.transitionmode2string(lighting._transition._mode))
            {
                lighting.set_transitionmode((string)cmbTransitionMode.SelectedItem);
                redraw();
            }
        }

        private void nmTransitionWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionWidth.Value == null)
            {
                nmTransitionWidth.Value = lighting._transition._width;
            }
            else {
                lighting.set_transitionwidth((byte)nmTransitionWidth.Value);
                redraw();
            }
        }

        private void nmTransitionSpeed_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionSpeed.Value == null)
            {
                nmTransitionSpeed.Value = lighting._transition._speed;
            }
            else
            {
                lighting.set_transitionspeed((byte)nmTransitionSpeed.Value, (ulong)nmTicks.Value);
                redraw();
            }
        }

        private void nmTransitionFlicker_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionFlicker.Value == null)
            {
                nmTransitionFlicker.Value = lighting._transition._flickeractivation;
            }
            else
            {
                lighting.set_transitionflickeractivation((byte)nmTransitionFlicker.Value);
                redraw();
            }
        }
        private Lighting GetSelectedLighting()
        {
            if (cmbPresets.SelectedValue == null) return null;
            return ((KeyValuePair<string, Lighting>)cmbPresets.SelectedItem).Value;
        }
        private void cmbColorArray_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            bool update = false;

            var colorArray = GetColorArrayForSelect();
            if (cmbColorArray.SelectedItem == null)
            {
                // should reset?
                if (colorArray.Value != null)
                {
                    cmbColorArray.SelectedItem = colorArray;
                }
                return;
            }
            if (cmbColorArray.SelectedItem == null)
            {
                if (colorArray.Value == null) return;
                cmbColorArray.SelectedItem = colorArray;
            }
            else if (((KeyValuePair<string, ulong[]>)cmbColorArray.SelectedItem).Key != colorArray.Key)
            {
                lighting.clear_colorarray();
                foreach(var c in ((KeyValuePair<string, ulong[]>)cmbColorArray.SelectedItem).Value)
                {
                    lighting.append_colorarray(c);
                }
                redraw();
            }

        }

        private void cmbTransitionChaseMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbTransitionChaseMode.SelectedItem != LightingChase.chasemode2string(lighting._transition._chase._mode))
            {
                lighting.set_transitionchasemode((string)cmbTransitionChaseMode.SelectedItem);
                redraw();
            }
        }

        private void cmbTransitionChaseTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbTransitionChaseTarget.SelectedItem != LightingChase.chasetarget2string(lighting._transition._chase._target))
            {
                lighting.set_transitionchasetarget((string)cmbTransitionChaseTarget.SelectedItem);
                redraw();
            }
        }

        private void nmTransitionChaseWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionChaseWidth.Value == null)
            {
                nmTransitionChaseWidth.Value = lighting._transition._chase._width;
            }
            else
            {
                lighting.set_transitionchasewidth((byte)nmTransitionChaseWidth.Value);
                redraw();
            }
        }

        private void cmbTransitionShiftMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbTransitionShiftMode.SelectedItem != LightingShift.shiftmode2string(lighting._transition._shift._mode))
            {
                lighting.set_transitionshiftmode((string)cmbTransitionShiftMode.SelectedItem);
                redraw();
            }
        }

        private void cmbTransitionShiftTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbTransitionShiftTarget.SelectedItem != LightingShift.shifttarget2string(lighting._transition._shift._target))
            {
                lighting.set_transitionshifttarget((string)cmbTransitionShiftTarget.SelectedItem);
                redraw();
            }
        }

        private void nmTransitionShiftAmount_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionShiftAmount.Value == null)
            {
                nmTransitionShiftAmount.Value = lighting._transition._shift._amount;
            }
            else
            {
                lighting.set_transitionshiftamount((byte)nmTransitionShiftAmount.Value);
                redraw();
            }
        }

        private void nmTransitionShiftStep_ValueChanged(object sender, EventArgs e)
        {
        }

        private void cmbFadeMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbFadeMode.SelectedItem != LightingFade.fademode2string(lighting._fade._mode))
            {
                lighting.set_fademode((string)cmbFadeMode.SelectedItem);
                redraw();
            }
        }

        private void nmFadeSpeed_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeSpeed.Value == null)
            {
                nmFadeSpeed.Value = lighting._fade._speed;
            }
            else
            {
                lighting.set_fadespeed((byte)nmFadeSpeed.Value, (ulong)nmTicks.Value);
                redraw();
            }
        }

        private void nmFadeFlicker_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeFlicker.Value == null)
            {
                nmFadeFlicker.Value = lighting._fade._flickeractivation;
            }
            else
            {
                lighting.set_fadeflickeractivation((byte)nmFadeFlicker.Value);
                redraw();
            }
        }

        private void cmbFadeChaseMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbFadeChaseMode.SelectedItem != LightingChase.chasemode2string(lighting._fade._chase._mode))
            {
                lighting.set_fadechasemode((string)cmbFadeChaseMode.SelectedItem);
                redraw();
            }
        }

        private void cmbFadeChaseTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbFadeChaseTarget.SelectedItem != LightingChase.chasetarget2string(lighting._fade._chase._target))
            {
                lighting.set_fadechasetarget((string)cmbFadeChaseTarget.SelectedItem);
                redraw();
            }
        }

        private void nmFadeChaseWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeChaseWidth.Value == null)
            {
                nmFadeChaseWidth.Value = lighting._fade._chase._width;
            }
            else
            {
                lighting.set_fadechasewidth((byte)nmFadeChaseWidth.Value);
                redraw();
            }
        }

        private void cmbFadeShiftMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbFadeShiftMode.SelectedItem != LightingShift.shiftmode2string(lighting._fade._shift._mode))
            {
                lighting.set_fadeshiftmode((string)cmbFadeShiftMode.SelectedItem);
                redraw();
            }
        }

        private void cmbFadeShiftTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if ((string)cmbFadeShiftTarget.SelectedItem != LightingShift.shifttarget2string(lighting._fade._shift._target))
            {
                lighting.set_fadeshifttarget((string)cmbFadeShiftTarget.SelectedItem);
                redraw();
            }
        }

        private void nmFadeShiftAmount_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeShiftAmount.Value == null)
            {
                nmFadeShiftAmount.Value = lighting._fade._shift._amount;
            }
            else
            {
                lighting.set_fadeshiftamount((byte)nmFadeShiftAmount.Value);
                redraw();
            }
        }

        private void nmFadeShiftStep_ValueChanged(object sender, EventArgs e)
        {

        }

        private void cmbFadeBgColor_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            var bgColor = GetBgColorForSelect();
            if (cmbFadeBgColor.SelectedItem == null)
            {
                if (bgColor.Value != null)
                {
                    cmbFadeBgColor.SelectedItem = bgColor;
                }
                return;
            }
            if (((KeyValuePair<string,ulong>)cmbFadeBgColor.SelectedItem).Value != lighting._fade._bgcolor)
            {
                lighting.set_bgcolor(((KeyValuePair<string, ulong>)cmbFadeBgColor.SelectedItem).Value);
                redraw();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            nmTicks.Value += 1;
        }

        private void nmFadeShiftWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeShiftWidth.Value == null)
            {
                nmFadeShiftWidth.Value = lighting._fade._shift._width;
            }
            else
            {
                lighting.set_fadeshiftwidth((byte)nmFadeShiftWidth.Value);
                redraw();
            }
        }

        private void nmTransitionShiftWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmTransitionShiftWidth.Value == null)
            {
                nmTransitionShiftWidth.Value = lighting._transition._shift._width;
            }
            else
            {
                lighting.set_transitionshiftwidth((byte)nmTransitionShiftWidth.Value);
                redraw();
            }
        }

        private void nmFadeWidth_ValueChanged(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            if (lighting == null) return;
            if (nmFadeWidth.Value == null)
            {
                nmFadeWidth.Value = lighting._fade._width;
            }
            else
            {
                lighting.set_fadewidth((byte)nmFadeWidth.Value);
                redraw();
            }

        }

        private void btnShowText_Click(object sender, EventArgs e)
        {
            var lighting = GetSelectedLighting();
            txtDebug.Text = lighting.displaySettings();
            txtHelp.Text = lighting.toString();
        }
    }
}