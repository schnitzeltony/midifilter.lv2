MFD_FILTER(velocityscale)

#ifdef MX_TTF

	mflt:velocityscale
	TTF_DEFAULTDEF("MIDI Velocity Adjust")
	, TTF_IPORT(0, "channel", "Channel",  0.0, 16.0,  0.0, PORTENUMZ("Any"))
	, TTF_IPORTFLOAT(1, "onmin",  "Note-on min",  1.0, 127.0,   1.0)
	, TTF_IPORTFLOAT(2, "onmax",  "Note-on max",  0.0, 127.0, 127.0)
	, TTF_IPORTFLOAT(3, "offmin", "Note-off min", 0.0, 127.0,   0.0)
	, TTF_IPORTFLOAT(4, "offmax", "Note-off max", 0.0, 127.0, 127.0)
	, TTF_IPORTFLOAT(5, "gain",   "common gain offset", -64.0,  64.0,   0.0)
	.

#elif defined MX_CODE

static void filter_init_velocityscale(MidiFilter* self) { }

static void
filter_midi_velocityscale(MidiFilter* self,
		uint32_t tme,
		const uint8_t* const buffer,
		uint32_t size)
{
	const int chs = midi_limit_chn(floor(*self->cfg[0]) -1);
	const uint8_t chn = buffer[0] & 0x0f;
	uint8_t mst = buffer[0] & 0xf0;

	if (size != 3
			|| !(mst == MIDI_NOTEON || mst == MIDI_NOTEOFF)
			|| !(floor(*self->cfg[0]) == 0 || chs == chn)
		 )
	{
		forge_midimessage(self, tme, buffer, size);
		return;
	}

	const uint8_t vel  = (buffer[2] & 0x7f);
	const float offset = *(self->cfg[5]);

	if (mst == MIDI_NOTEON && vel == 0 ) {
		mst = MIDI_NOTEOFF;
	}

	uint8_t buf[3];
	buf[0] = buffer[0];
	buf[1] = buffer[1];


	switch(mst) {
		case MIDI_NOTEON:
			{
				const float nmin = *(self->cfg[1]);
				const float nmax = *(self->cfg[2]);
				const float oneoff = (nmax-nmin)/126.0;
				buf[2] = RAIL(rintf((float)vel * (nmax-nmin) / 126.0  + nmin - oneoff + offset), 1, 127);
			}
			break;
		case MIDI_NOTEOFF:
			{
				const float nmin = *(self->cfg[3]);
				const float nmax = *(self->cfg[4]);
				buf[2] = RAIL(rintf((float)vel * (nmax-nmin) / 127.0  + nmin + offset), 0, 127);
			}
			break;
	}
	forge_midimessage(self, tme, buf, size);
}

#endif