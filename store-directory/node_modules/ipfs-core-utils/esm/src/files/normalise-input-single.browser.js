import { normaliseContent } from './normalise-content.browser.js';
import { normaliseCandidateSingle } from './normalise-candidate-single.js';
export function normaliseInput(input) {
  return normaliseCandidateSingle(input, normaliseContent);
}