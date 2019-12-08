
const fs = require('fs'); const
  path = require('path');

function walk(dir) {
  if (!fs.existsSync(dir)) {
    return [];
  }

  return fs.readdirSync(dir).filter((f) => f && f[0] !== '.')
    .map((f) => {
      const p = path.join(dir, f); const
        stat = fs.statSync(p);

      if (stat.isDirectory()) {
        return {
          name: f,
          type: 'folder',
          path: p,
          items: walk(p)
        };
      }

      return {
        name: f,
        type: 'file',
        path: p,
        size: stat.size
      };
    });
}


module.exports = function scan(dir) {
  return {
    name: dir,
    type: 'folder',
    path: dir,
    items: walk(dir)
  };
};
