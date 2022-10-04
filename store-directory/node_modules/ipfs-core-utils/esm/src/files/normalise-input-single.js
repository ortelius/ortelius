import { normaliseContent } from './normalise-content.js';
import { normaliseCandidateSingle } from './normalise-candidate-single.js';
export function normaliseInput(input) {
  return normaliseCandidateSingle(input, normaliseContent);
}