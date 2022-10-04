import { normaliseContent } from './normalise-content.browser.js';
import { normaliseCandidateMultiple } from './normalise-candidate-multiple.js';
export function normaliseInput(input) {
  return normaliseCandidateMultiple(input, normaliseContent, true);
}