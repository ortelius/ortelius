const toPathComponents = (path = '') => {
  return (path.trim().match(/([^\\/]|\\\/)+/g) || []).filter(Boolean);
};
export default toPathComponents;