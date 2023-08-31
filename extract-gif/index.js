const sharp = require('sharp');
const gifFrames = require('gif-frames');
const fs = require('fs');
const fn = 'mario';

function extractFrames() {
  return new Promise((resolve) => {
    return gifFrames({
      url: `img/${fn}.gif`,
      frames: 'all',
      quality: 100,
      type: 'gif',
      cumulative: true,
    })
      .then(framesData => {
        const q = framesData.map((n, i) => new Promise(res => {
          const filename = `${fn}-${i}.gif`;
          const fd = `img-out/`;
          const writable = fs.createWriteStream(`${fd}${filename}`);
          n.getImage().pipe(writable);
          writable.on('finish', () => res(filename));
        }));

        return Promise.all(q)
          .then(filePaths => {
            resolve(filePaths);
          });
      });
  });
}

function resizeFrames(filesPaths) {
  const q = filesPaths.map(fn => new Promise(res => sharp(`img-out/${fn}`)
    .resize(16, 16, {kernel: 'cubic', fit: 'cover'})
    .jpeg({quality: 100})
    .toFile(`img-res/${fn}`, (err, info) => {
      if (err) {
        throw Error(err);
      }

      return res(fn);
    })));

  return Promise.all(q);
}

function getFrame(path) {
  return new Promise(res => {
    sharp(path)
      .raw()
      .toBuffer((err, data, info) => {
        return res({ err, data, info });
      });
  });
}

function getFramesData(fileNames) {
  const paths = fileNames
    .map(n => `img-res/${n}`);

  const frames = paths
    .map(async (path) => await getFrame(path));

  return Promise.all(frames);
}

async function convertFramesToPixelsArrays(fileNames) {
  return getFramesData(fileNames)
    .then(framesData => {
      return framesData.map(({err, info, data}) => {
        if (err) {
          return new Error();
        }

        let frame = [];
        const width = info.width;
        const height = info.height;
        const channels = info.channels; // Typically 3 for RGB

        for (let y = 0; y < height; y++) {
          let row = [];

          for (let x = 0; x < width; x++) {
            const pixelIndex = (y * width + x) * channels;

            const r = data[pixelIndex];
            const g = data[pixelIndex + 1];
            const b = data[pixelIndex + 2];

            row = [...row, `strip.Color(${r},${g},${b})`];
          }

          frame = [...frame, row];
        }

        return frame;
      });
    });
}

function covertToArduinoArray(fileNames) {
  return convertFramesToPixelsArrays(fileNames)
    .then(res => {
      const framesCount = res.length;

      let str = `int ${fn}FramesCount = ${framesCount};\nuint32_t ${fn}[${framesCount}][16][16] = {`;

      str += res
        .reduce((frameStr, frame) => {
          let _frameStr = frame
            .reduce((acc, row) => `${acc}\n\t\{${row.join(', ')}\},`, '')
            .replace(/,\s*$/, '');

          return `${frameStr}\n\{${_frameStr}\n\},`;
        }, '')
        .replace(/,\s*$/, '');

      str += '\n};'

      return str;
    });
}

extractFrames()
  .then(filePaths => resizeFrames(filePaths))
  .then(fileNames => covertToArduinoArray(fileNames))
  .then(res => console.log(res));