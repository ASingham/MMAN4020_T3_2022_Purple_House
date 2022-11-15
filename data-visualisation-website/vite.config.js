/* eslint-disable */
import react from '@vitejs/plugin-react';
import { defineConfig } from 'vite';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  resolve: {
    alias: {
      '@mui/styled-engine': '@mui/styled-engine-sc'
    }
  },
  server: {
    port: 3000
  }
})
