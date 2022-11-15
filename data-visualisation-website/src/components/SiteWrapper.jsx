import { styled } from '@mui/material/styles';

const BodyWrapper = styled('div')`
  display: flex;
  flex-direction: column;
  background-color: ${ props => props.theme.background };
  color: ${ props => props.theme.text };
  min-height: calc(100vh - 64px);
`;

const SiteWrapper = ({ children }) => {
  return (
    <>
      <BodyWrapper>
        { children }
      </BodyWrapper>
    </>
  );
};

export default SiteWrapper;
