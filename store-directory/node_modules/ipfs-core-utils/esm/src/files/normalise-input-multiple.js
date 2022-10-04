import { normaliseContent } from './normalise-content.js';
import { normaliseCandidateMultiple } from './normalise-candidate-multiple.js';
export function normaliseInput(input) {
  return normaliseCandidateMultiple(input, normaliseContent);
}