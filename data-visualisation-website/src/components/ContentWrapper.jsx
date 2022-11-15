import { styled } from '@mui/material/styles';

export const PageContent = styled('main')`
  flex-grow: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  & > * {
    width: 85%;
    max-width: 900px;
  };
  & > *:first-child {
    margin-top: 1rem;
  }
  & > *:not(:last-child) {
    margin-bottom: calc(1rem * 0.5);
  }
  & > *:last-child {
    margin-bottom: 1rem;
  }
`;

export const CenterPageContent = styled('main')`
  flex-grow: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  & > * {
    width: 50%;
    max-width: 700px;
  };
`;

const ContentWrapper = ({ center, children }) => {
  if (center) {
    return (
      <CenterPageContent>
        { children }
      </CenterPageContent>
    );
  }

  return (
    <PageContent>
      { children }
    </PageContent>
  );
};

export default ContentWrapper;
