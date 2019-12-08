module.exports = {
  env: {
    "node": true,
    "commonjs": true,
    "es6": true
  },
  extends: ["airbnb"],
  parserOptions: {
    "ecmaVersion": 2018,
    "sourceType": "module"
  },
  rules: {
    "indent": ["error", 2],
    "linebreak-style": ["error", "windows"],
    "quotes": ["error", "single"],
    "semi": ["error", "always"],
    "comma-dangle": ["error", "never"],
    "no-underscore-dangle": 0,
    "no-plusplus": 0,
    "no-param-reassign": 0,
    "no-console": 0
  }
}
