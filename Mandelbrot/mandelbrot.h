const unsigned int W = 1920;
const unsigned int H = 1080;

const float RE_START = -1;
const float IM_START = 0;

const float INITIAL_RE_WIDTH = 2;

const float RE_WIDTH = INITIAL_RE_WIDTH;
const float IM_WIDTH = INITIAL_RE_WIDTH * H / W;

const float INITIAL_SCALE = 1.0f;
const float INITIAL_RE_STEP = 2 * RE_WIDTH / W * INITIAL_SCALE;
const float INITIAL_IM_STEP = 2 * IM_WIDTH / H * INITIAL_SCALE;

const size_t N_MAX_VALUE = 256;
const float MAX_ABS = 50;

const float MOVE_COEF = 0.05f;
