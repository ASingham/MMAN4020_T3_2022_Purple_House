import Alert from '@mui/material/Alert';
import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import CircularProgress from '@mui/material/CircularProgress';
import FormControl from '@mui/material/FormControl';
import Input from '@mui/material/Input';
import Papa from 'papaparse';
import { useEffect, useRef, useState } from 'react';

import CSVParser from '../CSVParser';
import Data from '../Data';
import styles from '../styles/styles.module.css';

const FileForm = ({ onSubmit }) => {
  const fileRef = useRef();
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [files, setFiles] = useState([]);
  const [parsedFiles, setParsedFiles] = useState([]);

  // Listen for files submitted
  useEffect(() => {
    if (files.length) parseFiles();
  }, [files]);

  // Wait for all files to be parsed
  useEffect(() => {
    if (parsedFiles.length > 0 && parsedFiles.length === files.length) {
      onSubmit(parsedFiles);
    }
  }, [parsedFiles]);

  const parseFiles = () => {
    if (!files.length) {
      setError('No file selected');
    } else if (files.length > 4) {
      setError('Too many files');
    } else {
      setError('');
      
      for (let i = 0; i < files.length; i++) {
        parseCSVFile(files[i]);
      }
    }
  };

  const handleSubmit = (event) => {
    event.preventDefault();
    setLoading(true);
    const filesArray = [];
    Object.keys(fileRef.current.files).forEach(i => {
      filesArray.push(fileRef.current.files[i]);
    });
    setFiles(filesArray);
  };

  const parseCSVFile = (file) => {
    const onComplete = (results) => {
      try {
        const newResults = CSVParser.parseAskSensors(file.name, results);
        setParsedFiles(prev => [...prev, new Data(newResults)]);
      } catch (e) {
        console.error(e);
        setLoading(false);
        setError('Unable to read file. Make sure it is an AskSensors CSV file');
      }
    };

    const onError = (error) => {
      console.error('Error:', error);
      setLoading(false);
    };
    
    const cfg = {
      complete: onComplete,
      error: onError,
      header: true,
      dynamicTyping: true,
      worker: true,
      delimiter: ';',
      skipEmptyLines: 'greedy'
    };

    Papa.parse(file, cfg);
  };

  if (loading) return (
    <Box sx={{ width: '100%', display: 'flex', justifyContent: 'center' }}>
      <CircularProgress color="secondary"/>
    </Box>
  );

  return (
    <FormControl sx={{ width: '100%' }} className={styles.verticalcontainer}>
      <Box className={styles.splitbox}>
        <Input id='csv-file-input' type='file' inputRef={fileRef} required={true} sx={{ width: '100%' }} color='secondary' inputProps={{ multiple: true, accept: '.csv' }}/>
        <Button variant='outlined' color='secondary' type='button' onClick={handleSubmit} sx={{ ml: 2 }}>Submit</Button>
      </Box>
      { error && <Alert severity='error'>{error}</Alert> }
    </FormControl>
  );
};

export default FileForm;
