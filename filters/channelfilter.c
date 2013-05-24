MFD_FILTER(channelfilter)

#ifdef MX_TTF

	mflt:channelfilter
	TTF_DEFAULTDEF("MIDI Channel Filter")
	, TTF_IPORTTOGGLE( 0, "chn1",  "Channel  1", 1.0)
	, TTF_IPORTTOGGLE( 1, "chn2",  "Channel  2", 1.0)
	, TTF_IPORTTOGGLE( 2, "chn3",  "Channel  3", 1.0)
	, TTF_IPORTTOGGLE( 3, "chn4",  "Channel  4", 1.0)
	, TTF_IPORTTOGGLE( 4, "chn5",  "Channel  5", 1.0)
	, TTF_IPORTTOGGLE( 5, "chn6",  "Channel  6", 1.0)
	, TTF_IPORTTOGGLE( 6, "chn7",  "Channel  7", 1.0)
	, TTF_IPORTTOGGLE( 7, "chn8",  "Channel  8", 1.0)
	, TTF_IPORTTOGGLE( 8, "chn9",  "Channel  9", 1.0)
	, TTF_IPORTTOGGLE( 9, "chn10", "Channel 10", 1.0)
	, TTF_IPORTTOGGLE(10, "chn11", "Channel 11", 1.0)
	, TTF_IPORTTOGGLE(11, "chn12", "Channel 12", 1.0)
	, TTF_IPORTTOGGLE(12, "chn13", "Channel 13", 1.0)
	, TTF_IPORTTOGGLE(13, "chn14", "Channel 14", 1.0)
	, TTF_IPORTTOGGLE(14, "chn15", "Channel 15", 1.0)
	, TTF_IPORTTOGGLE(15, "chn16", "Channel 16", 1.0)
	.

#elif defined MX_CODE

void filter_init_channelfilter(MidiFilter* self) { }

void
filter_midi_channelfilter(MidiFilter* self,
		uint32_t tme,
		const uint8_t* const buffer,
		uint32_t size)
{
	if (size > 3) {
		forge_midimessage(self, tme, buffer, size);
		return;
	}

	int chn = buffer[0]&0x0f;
	switch (buffer[0] & 0xf0) {
		case 0x80: // note off
		case 0x90: // note on
		case 0xA0: // Polyphonic Key Pressure (Aftertouch)
		case 0xB0: // control change
		case 0xC0: // program change
		case 0xD0: // Channel Pressure (After-touch)
		case 0xE0: // pitch wheel
			if (*(self->cfg[chn])) {
				forge_midimessage(self, tme, buffer, size);
			}
			break;
		default:
			forge_midimessage(self, tme, buffer, size);
			break;
	}
}

#endif