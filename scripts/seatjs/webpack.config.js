const path = require('path');

module.exports = {
    entry: {
        seatjs_api: './src/seatjs-api.ts',
        seatjs_jsonrpc: './src/seatjs-jsonrpc.ts',
        seatjs_rpcerror: './src/seatjs-rpcerror.ts',
        seatjs_jssig: './src/seatjs-jssig.ts',
    },
    devtool: 'inline-source-map',
    mode: 'development',
    output: {
        library: 'seatjs',
        pathinfo: true,
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: {
                    loader: 'ts-loader',
                    options: {
                        configFile: 'tsconfig.json'
                    }
                },
                exclude: /node_modules/,
            }
        ]
    },
    resolve: {
        extensions: ['.tsx', '.ts', '.js']
    },
    output: {
        filename: x => x.chunk.id.replace('_', '-') + '-debug.js',
        library: '[id]',
        path: path.resolve(__dirname, 'dist-web'),
    }
};
